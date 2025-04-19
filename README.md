Simple C++ app to test the hypothesis: if I record the length of periods of
uninterrupted focus and give myself positive reinforcement for longer periods,
I should be able to train my mind to focus for long periods of time more easily.

### Installation and Setup
Made to work on Wayland.
To compile, run:

```
clang++ main.cpp -linput -ludev -o attinv
```

This uses `libinput` and thus requires sudo to run:

```
sudo attinv path/to/conf/file.conf
```

Currently the config file expects one line:

```
path/to/log/dir/
```

### Usage

Run with:

```
sudo attinv path/to/conf/file.conf
```

To log a period of focus, use `Ctrl-Super-P`.


### Config Specification

The currently supported config options are `log_dir` and `log_hotkey`.
- `log_dir` should be set to a string value representing a directory.
- `log_hotkey` should be set to a string value representing a hotkey combination. The format should look like `Ctrl-Super-P` with keys separated by -.

Each non-empty line of the config is expected to have the following syntax:

```
option_name = option_value
```

### TODOs
- Figure out using supplementary files (my keycodes file) for stuff with CMake
- Write tests?
- Support MacOS.
- Look into how to make it appear in the top bar dock.

