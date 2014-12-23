CALIDAR
=======

Code for automated calibration of spinning lidar internal parameters.

Installation
---

Code relies on Matlab for optimization (requires Matlab Optimization Toolbox). Other depedencies include:

- Eigen (version 3+) http://eigen.tuxfamily.org/index.php?title=Main_Page
- nanoflann https://github.com/jlblancoc/nanoflann
- mexmat https://github.com/halismai/mexmat
- g++ 4.7+ (for c++-11 support)
- tbb (optional, but highly recommended)

NOTE: nanoflann can be optional. Edit utils/@KdTree/KdTree constructor and set
```obj.use_nanoflann = false```
we recommended using nanoflann.


For question, comments, or bug reports please contact:

Hatem Alismail <halismai @ cs.cmu.edu>


Usage
---
Code was designed to accomodate various sensor configurations. In particular,
code is not tied to a specific coordinate convention.

For an example see the file ```example/run_example.m```, which contains:

```matlab
opts = struct('actuation_func', @example_sensor.actuate_split, ...
  'p2h', @example_model.p2h, ...
  'h2p', @example_model.h2p, ...
  'compute_delta_params', @example_model.compute_delta_params, ...
  'H_init', eye(4), ...
  'do_show', true, ...
  'max_outer_iters', 10);
```

This struct specifies options to the calibration routine. Most important fields:

- ```actuation_func``` a function supplied by the user to perform actuation of
  the lidar sensing assembly

- ```p2h``` Parameters to Homogenous transformation. Takes a vector of
  parameters and converts it into a 4x4 homogenous transformation matrix.

- ```h2p``` Homogenous transformation matrix to Parameters vector. Takes a 4x4
  matrix and converts back to a vector of optimization parameters.

- ```H_init``` User supplied initializing for the calibration parameters (4x4
  matrix). This is required if the initial offsets are large. The value need not
  be accurate and usually eye-balling, or nominal values from a CAD model
  will suffice.

There are other options fully documented in ```calibration/private/verify_options.m```

See the implementation of example_sensor in ```+example_sensor``` and
```+example_model```. The implementation is put into a matlab package so that
other sensors can share the same internal function names.

For the example sensor, data is stored in an ascii CSV file. Your data can be
stored in any format if you provide the I/O routines.


Recommendations for data collection
---

- In order to calibration the lidar you need at least one full revolution (360
  degrees) scan. More revolutions is better. We usually collect between 2-4.

- There are no strict restrictions on the data collection environment. But, the
  environment must have enough 3D structure to constrain the full degrees of
  freedom (DOF) of the sensing assembly.

  An ideal data collection location would be a large room with several planes of
  different directions.

  A bad data collection location would be scanning the floor of a room. A single
  plane direction does not constraint the full DOF.


- Speed/acceleration rate of the spinning motor does not affect the calibration
  significantly (in fact calibration parameters are indepdent of motor
  configuration). However, the spinning speed must be sufficiently slow to allow
  extraction of normals.

  If the lidar spins fast relative to the environment, scans might cluster along
  lines without enough constrains to extract normals.

Please see references below for more details

Citation
----
If you use this software please cite:

- H. Alismail, and B. Browning, "Automatic Calibration of Spinning Actuated
  Lidar Internal Parameters." Journal of Field Robotics. 2014

BibTex entry:

```
@article{ROB:ROB21543,
  author = {Alismail, Hatem and Browning, Brett},
  title = {Automatic Calibration of Spinning Actuated Lidar Internal Parameters},
  journal = {Journal of Field Robotics},
  issn = {1556-4967},
  url = {http://dx.doi.org/10.1002/rob.21543},
  doi = {10.1002/rob.21543},
  pages = {n/a--n/a},
  year = {2014},
  }
```

or

- H. Alismail, L.D. Baker, and B. Browning, "Automatic Calibration of a Range Sensor and Camera System." 3DIMPVT, 2012.


Bib entry:
```
@INPROCEEDINGS{6375006,
author={Alismail, H. and Baker, L.D. and Browning, B.},
booktitle={3D Imaging, Modeling, Processing, Visualization and Transmission (3DIMPVT), 2012 Second International Conference on},
title={Automatic Calibration of a Range Sensor and Camera System},
year={2012},
month={Oct},
pages={286-292},
doi={10.1109/3DIMPVT.2012.52}}
```

