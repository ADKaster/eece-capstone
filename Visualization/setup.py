#!/usr/bin/python

"""
Installation Instruction:
in command propmpt, wth admin proveleges, run the following commands:
    setup.py build
    setup.py install
"""

try:
    from setuptools import setup
except ImportError:
    from distutils.core import setup

config = {
    'description': 'Data Visualization for Payload Data',
    'author': 'Annabel Lewis',
    'url': '',
    'download_url': '',
    'author_email': '',
    'version': '1.0.0',
    'install_requires': [
        'matplotlib==2.2.2',
        'pyserial==3.4',
    ],
    'scripts': ['main.py'],
    'data_files': [],
}

setup(**config)
