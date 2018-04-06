import numpy as np
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
import matplotlib.animation as animation
import serial
import io
from time import time
from random import randint


class SubplotAnimation(animation.TimedAnimation):
    def __init__(self):
        self.start_time = time()
        self.data_dict = {
            "temperature": ([], []),
            "pressure": ([], []),
            "altitude": ([], [])
        }
        self.max_points = 300

        # establish subplots
        self.fig = plt.figure()
        self.axes_altitude = self.fig.add_subplot(1, 2, 1)
        self.axes_temperature = self.fig.add_subplot(1, 2, 2)

        # configure altitude subplot
        self.line_altitude = Line2D([], [], color='blue')
        self.axes_altitude.add_line(self.line_altitude)
        self.axes_altitude.set_xlim(0, self.max_points/5)
        self.axes_altitude.set_ylim(0, 10)
        self.axes_altitude.set_title('Altitude v Time')
        self.axes_altitude.set_xlabel('Time (s)')
        self.axes_altitude.set_ylabel('Altitude (m)')

        # configure temperature subplot
        self.line_temperature = Line2D([], [], color='green')
        self.axes_temperature.add_line(self.line_temperature)
        self.axes_temperature.set_xlim(0, self.max_points/5)
        self.axes_temperature.set_ylim(60, 80)
        self.axes_temperature.set_title('Temperature v Time')
        self.axes_temperature.set_xlabel('Time (s)')
        self.axes_temperature.set_ylabel('Temperature (s)')

        # start animation
        animation.TimedAnimation.__init__(self, self.fig, interval=50, blit=True)

    def _draw_frame(self, _):
        self.serial_read()

        self.axes_altitude, self.line_altitude = self.adjust_dataset('altitude', self.axes_altitude, self.line_altitude)
        self.axes_temperature, self.line_temperature = self.adjust_dataset(
            'temperature', self.axes_temperature, self.line_temperature
        )

        self._drawn_artists = [self.line_altitude, self.line_temperature]

    def adjust_dataset(self, category_str, axes, line):
        # trim dataset to be within the max number of points
        # max points can be changes with some other benchmark (time elapsed?)
        while len(self.data_dict[category_str][0]) > self.max_points:
            self.data_dict[category_str][0].pop(0)
            self.data_dict[category_str][1].pop(0)
            axes.set_xlim(self.data_dict[category_str][0][0], self.data_dict[category_str][0][-1], auto=True)
        line.set_data(self.data_dict[category_str][0], self.data_dict[category_str][1])

        return axes, line

    def new_frame_seq(self):
        # required for implementation; change iterable to something useful if need be
        self.serial_read()
        return iter(range(1))

    def _init_draw(self):
        lines = [self.line_altitude, self.line_temperature]
        for l in lines:
            l.set_data([], [])

    def serial_read(self):
        ser = serial.serial_for_url('loop://', timeout=0.1)

        serial_writer = io.TextIOWrapper(io.BufferedWriter(ser))

        serial_writer.write('alt,{0}\n'.format(randint(1, 10)))
        serial_writer.flush()
        serial_writer.write('tem,{0}\n'.format(randint(67, 73)))
        serial_writer.flush()
        serial_reader = io.TextIOWrapper(io.BufferedReader(ser))

        try:
            line_in = serial_reader.readline()
            while line_in != '':
                line_split = line_in.split(',')
                if line_split[0] == 'alt':
                    self.data_dict['altitude'][0].append(time()-self.start_time)
                    self.data_dict['altitude'][1].append(int(line_split[1]))
                elif line_split[0] == 'tem':
                    self.data_dict['temperature'][0].append(time() - self.start_time)
                    self.data_dict['temperature'][1].append(int(line_split[1]))
                line_in = serial_reader.readline()
        except ValueError:
            pass


if __name__ == '__main__':
    ani = SubplotAnimation()
    plt.show()
