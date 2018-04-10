import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import matplotlib.animation as animation
import serial
import io
from time import time
from random import randint
from subprocess import Popen
import threading
from math import sqrt


class SubplotAnimation(animation.TimedAnimation):
    def __init__(self):
        self.start_time = time()
        self.data_dict = {
            "acceleration": ([0], [0]),
            "altitude": ([0], [0])
        }
        self.display_time = 60

        # establish subplots
        self.fig = plt.figure()
        self.axes_altitude = self.fig.add_subplot(1, 2, 1)
        self.axes_acceleration = self.fig.add_subplot(1, 2, 2)

        # configure altitude subplot
        self.line_altitude = Line2D([], [], color='blue')
        self.axes_altitude.add_line(self.line_altitude)
        self.axes_altitude.set_xlim(0, self.display_time)
        self.axes_altitude.set_ylim(-70, -60)
        self.axes_altitude.set_title('Altitude v Time')
        self.axes_altitude.set_xlabel('Time (s)')
        self.axes_altitude.set_ylabel('Altitude (m)')

        # configure acceleration subplot
        self.line_acceleration = Line2D([], [], color='purple')
        self.axes_acceleration.add_line(self.line_acceleration)
        self.axes_acceleration.set_xlim(0, self.display_time)
        self.axes_acceleration.set_ylim(-20, 20)
        self.axes_acceleration.set_title('Acceleration v Time')
        self.axes_acceleration.set_xlabel('Time (s)')
        self.axes_acceleration.set_ylabel('Newtons (N)')

        # plt.subplots_adjust(top=0.92, bottom=0.08, left=0.10, right=0.95, hspace=0.25,
        #                     wspace=0.35)
        # start animation
        animation.TimedAnimation.__init__(self, self.fig, interval=1, blit=True)

    def _draw_frame(self, _):
        # self.serial_read()

        self.axes_altitude, self.line_altitude = self.adjust_dataset('altitude', self.axes_altitude, self.line_altitude)
        self.axes_acceleration, self.line_acceleration = self.adjust_dataset(
            'acceleration', self.axes_acceleration, self.line_acceleration
        )

        self._drawn_artists = [self.line_altitude, self.line_acceleration]

    def adjust_dataset(self, category_str, axes, line):
        # trim dataset to be within the max number of points
        # max points can be changes with some other benchmark (time elapsed?)
        while (self.data_dict[category_str][0][-1] - self.data_dict[category_str][0][0]) > self.display_time:
            self.data_dict[category_str][0].pop(0)
            self.data_dict[category_str][1].pop(0)
        axes.set_xlim(self.data_dict[category_str][0][0], self.data_dict[category_str][0][-1], auto=True)
        line.set_data(self.data_dict[category_str][0], self.data_dict[category_str][1])

        return axes, line

    def new_frame_seq(self):
        # required for implementation; change iterable to something useful if need be (like a cyclical timing sequence)
        # self.serial_read()
        return iter(range(1))

    def _init_draw(self):
        lines = [self.line_altitude, self.line_acceleration]
        for l in lines:
            l.set_data([], [])

def serial_read(data_dict):
    """
    0: tag (ALT|ACCEL)
    1: time seconds
    2: time nsec
    3: alt / accel-0
    4: accel-1
    5: accel-2
    """
    ser = serial.Serial('COM3', 115200, timeout=0.4)
    while True:
        # ser = serial.Serial('COM3', 115200, timeout=0.215)
        lines_in = []
        lines_in.append('0')
        lines_in.append('0')

        while lines_in[0] != '':
            lines_in[0] = ser.readline()[1:].decode()
            lines_in[1] = ser.readline()[1:].decode()

            for line_in in lines_in:
                print(line_in)
                line_in = line_in.lstrip()
                line_in = line_in.rstrip()
                line_split = line_in.split(',')

                if line_split[0] != '':
                    time_sec = line_split[1]
                    time_nsec = '0.{0}'.format(line_split[2])

                    if line_split[0] == 'LT' or line_split[0] == 'ALT':
                        data_dict['altitude'][0].append(float(time_sec)+float(str(time_nsec)))
                        data_dict['altitude'][1].append(float(line_split[3]))

                    elif line_split[0] == 'ACCEL':
                        accel_0_sq = float(line_split[3])*float(line_split[3])
                        accel_1_sq = float(line_split[4])*float(line_split[4])
                        accel_2_sq = float(line_split[5])*float(line_split[5])
                        accel_total = sqrt(accel_0_sq + accel_1_sq + accel_2_sq)
                        data_dict['acceleration'][0].append(int(time_sec)+float(str(time_nsec)))
                        data_dict['acceleration'][1].append(accel_total)


class serialThread (threading.Thread):
    def __init__(self, data_dict):
       threading.Thread.__init__(self)
       self.data_dict = data_dict

    def run(self):
       serial_read(self.data_dict)

if __name__ == '__main__':
    ani = SubplotAnimation()
    serial_thread = serialThread(ani.data_dict)
    serial_thread.start()
    plt.show()
