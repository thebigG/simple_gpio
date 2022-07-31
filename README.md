[![x86 Linux Build](https://github.com/thebigG/simple_gpio/actions/workflows/ci.yaml/badge.svg)](https://github.com/thebigG/simple_gpio/actions/workflows/ci.yaml)  
Simple to use GPIO library for linux

# Build
```
docker pull thebigg1/simple_gpio-linux-ci:latest 
sudo docker run -it thebigg1/simple_gpio-linux-ci:latest bash
git clone --recursive https://github.com/thebigG/simple_gpio.git
cd simple_gpio/
mkdir build
cd build/
cmake ..
make
```

# Usage
```
./simple_gpio_example 
Usage:
  -h [ --help ] arg      Show help. If given, show help on the specified topic.

Write to pin.:
  -p [ --Pin ] arg       GPIO PIN.
  -v [ --Value ] arg     GPIO PIN value:{1, 0}.

Simple front-end for linux GPIO driver.
Configured for:"Raspberry Pi 4 Model B" 
Pins available:[5,6,16,17,19,20,21,27,22,23,24,25,26,]

```

# Build as a library
```
docker pull thebigg1/simple_gpio-linux-ci:latest
sudo docker run -it thebigg1/simple_gpio-linux-ci:latest bash
git clone --recursive https://github.com/thebigG/simple_gpio.git
cd simple_gpio/simple_gpio/
mkdir build
cd build/
cmake ..
make
```
