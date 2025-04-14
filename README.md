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

To log a period of focus, use `<Ctrl-Super> P`.


### TODOs
- Support customized hotkeys.
- Organize code better.
- Support MacOS.
    - Add `cmake` build script.
