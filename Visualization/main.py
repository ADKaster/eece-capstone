"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
import serial
import io
from time import time
from random import randint

def animated_figure():

    def serial_read():

        ser = serial.serial_for_url('loop://', timeout=1)

        serial_writer = io.TextIOWrapper(io.BufferedWriter(ser))

        serial_writer.write('alt,{0}\n'.format(randint(1, 10)))
        serial_writer.flush()

        serial_reader = io.TextIOWrapper(io.BufferedReader(ser))

        line_in = serial_reader.readline()
        line_split = line_in.split(',')
        if line_split[0] == 'alt':
            data_dict['altitude'][0].append(time()-start_time)
            data_dict['altitude'][1].append(int(line_split[1]))

    # def serial_write():
    #     serial_writer = io.TextIOWrapper(io.BufferedWriter(ser))
    #
    #     serial_writer.write('alt,{0}\n'.format(randint(1, 10)))
    #     serial_writer.flush

    start_time = time()
    max_points = 30
    # First set up the figure, the axis, and the plot element we want to animate
    fig = plt.figure()
    ax = plt.axes(xlim=(0, max_points), ylim=(0, 10))
    line_altitude, = ax.plot([], [], lw=2)

    data_dict = {
        "temperature": ([], []),
        "pressure": ([], []),
        "altitude": ([], [])
    }

    # initialization function: plot the background of each frame
    def init():
        line_altitude.set_data([], [])
        return line_altitude,

    # animation function.  This is called sequentially
    def animate(i):
        # serial_write()
        serial_read()
        # x = np.linspace(0, 2, 1000)
        # y = np.sin(2 * np.pi * (x - 0.01 * i))
        while len(data_dict['altitude'][0]) > max_points:
            data_dict['altitude'][0].pop(0)
            data_dict['altitude'][1].pop(0)
            ax.set_xlim(data_dict['altitude'][0][0], data_dict['altitude'][0][-1], auto=True)

        else:
            x = data_dict['altitude'][0]
            y = data_dict['altitude'][1]
        line_altitude.set_data(x, y)
        return line_altitude,

    # call the animator.  blit=True means only re-draw the parts that have changed.
    anim = animation.FuncAnimation(fig, animate, init_func=init,
                                   frames=200, interval=10, blit=False)

    # save the animation as an mp4.  This requires ffmpeg or mencoder to be
    # installed.  The extra_args ensure that the x264 codec is used, so that
    # the video can be embedded in html5.  You may need to adjust this for
    # your system: for more information, see
    # http://matplotlib.sourceforge.net/api/animation_api.html
    # anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])

    plt.show()





def experiment_serial():
    ser = serial.serial_for_url('loop://', timeout=1)
    # ser = serial.Serial(port=None)
    sio = io.TextIOWrapper(io.BufferedRWPair(ser, ser))

    sio.write("hello\n")
    sio.flush()     # it is buffering. required to get the data out *now*
    hello = sio.readline()
    print(hello == "hello\n")


if __name__ == "__main__":
    # experiment_serial()
    animated_figure()

