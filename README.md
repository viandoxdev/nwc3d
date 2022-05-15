# C app for epsilon

## Build the app

You need to install an embedded ARM toolchain (with hardfloat support) and a couple Python modules.
The last command has to be executed with the targeted device plugged to the computer.

### Gentoo

setup the toolchain with [crossdev](https://wiki.gentoo.org/wiki/Crossdev) (make sure to follow the instruction to setup the crossdev overlay). Then build the toolchain (taken from [here]https://wiki.gentoo.org/wiki/ARM#Enable_hardfloat_support()):

```shell
# crossdev --stable -t arm-hardfloat-eabi --env \
           'EXTRA_ECONF="--with-cpu=cortex-m7 # important
                         --with-float-abi=hard 
                         --with-mode=thumb"'
```

To finish, install both `dev-python/lz4` and `dev-python/pypng` (used for the icon inlining).

### General install

```shell
brew install numworks/tap/arm-none-eabi-gcc # Or equivalent on your OS
pip3 install lz4 pypng
make clean && make run
```

## Run the app

The app is sent over to the calculator using the DFU protocol over USB.

```shell
brew install dfu-util # Or equivalent on your OS
# Now connect your NumWorks calculator to your computer using the USB cable
make run
```

## Notes

The NumWorks calculator runs [Epsilon](http://github.com/numworks/epsilon), a tailor-made calculator operating system. Starting from version 16, Epsilon allows installing custom binary apps. To run this sample app, make sure your calculator is up-to-date by visiting https://my.numworks.com. Note that at the moment Epsilon 16 is in beta, so you'll need to register as [a beta tester](https://my.numworks.com/user/beta).

Epsilon expects apps to follow a certain layout in memory. Namely, they should start with the following header:

|Offset| Size | Value      | Description                  |
|------|------|------------|------------------------------|
| 0x00 | 0x04 | 0xDEC0BEBA | Magic start-of-header marker |
| 0x04 | 0x04 | 0x00000000 | API Level |
| 0x08 | 0x04 | -          | Offset from start of the app to a NULL-terminated NFKD UTF-8 string containing the app name |
| 0x0C | 0x04 | -          | Size of the icon data |
| 0x10 | 0x04 | -          | Offset from start of the app to the actual icon data. This data should be the result of LZ4-compressing a sequence of 55x56 RGB565 pixels |
| 0x14 | 0x04 | -          | Offset from start of the app to the entry point. |
| 0x18 | 0x04 | -          | Size of the entire app |
| 0x22 | 0x04 | 0xDEC0BEBA | Magic end-of-header marker |

Generating the appropriate header is taken care of by a [linker script](/eadk/eadk.ld) when you run `make build`.

The information about where to link in flash and in RAM are dynamically extracted from headers provided by the calculator. The RAM is mapped from 0x2000 0000 and starts with the following header:

|Offset| Size | Value      | Description                  |
|------|------|------------|------------------------------|
| 0x00 | 0x04 | 0xEFEEDBBA | Magic start-of-header marker |
| 0x04 | 0x04 | -          | Pointer to kernel header |
| 0x08 | 0x04 | -          | Pointer to userland header |
| 0x0C | 0x04 | 0xEFEEDBBA | Magic end-of-header marker |

The userland header gives the following information useful to link the external application:

|Offset| Size | Value      | Description                  |
|------|------|------------|------------------------------|
| 0x00 | 0x04 | 0xDEC0EDFE | Magic start-of-header marker |
| 0x04 | 0x04 | -          | Version of epsilon that should be matched by kernel |
| 0x08 | 0x04 | -          | Storage address in RAM |
| 0x0C | 0x04 | -          | Storage size |
| 0x10 | 0x04 | -          | External applications start address in flash |
| 0x14 | 0x04 | -          | External applications end address in flash |
| 0x18 | 0x04 | -          | External applications start address in RAM |
| 0x1C | 0x04 | -          | External applications end address in RAM |
| 0x20 | 0x04 | 0xDEC0EDFE | Magic end-of-header marker |

This dynamic linking process is taken care of by [get_device_information.py](/eadk/get_device_information.py) script.

Once the corresponding binary is built on your computer, you will need to install it in your calculator's Flash memory. The included [run.py](/eadk/run.py) script will take care of this for you when you call `make run`.

Due to the embedded nature of Epsilon, this C++ app is built using `-ffreestanding -nostdinc -nostdlib`. The interface that an app can use to interact with the OS is essentially a short list of system calls. Feel free to browse the [code of Epsilon](http://github.com/numworks/epsilon) itself if you want to get an in-depth look.

Please note that any custom app is removed when resetting the calculator.

## License

This app is distributed under the terms of the BSD License. See LICENSE for details.

## Trademarks

NumWorks is a registered trademark.

