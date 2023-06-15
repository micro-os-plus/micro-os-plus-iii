# README-MAINTAINER

## micro-os-plus-iii

- <https://github.com/micro-os-plus/micro-os-plus-iii>

```sh
rm -rf ~/Work/micro-os-plus-iii/micro-os-plus-iii.git && \
mkdir -p ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii.git
```

or

```sh
git -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git pull
```

Satisfy dependencies for all configurations and run all tests:

```sh
xpm run install-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git

xpm run test-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git
```

## build-helper

- <https://github.com/micro-os-plus/build-helper-xpack>

```sh
rm -rf ~/Work/micro-os-plus/build-helper-xpack.git && \
mkdir -p ~/Work/micro-os-plus && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/build-helper-xpack.git \
  ~/Work/micro-os-plus/build-helper-xpack.git

xpm link -C ~/Work/micro-os-plus/build-helper-xpack.git
```

or

```sh
git -C ~/Work/micro-os-plus/build-helper-xpack.git pull
```

## posix-arch

- <https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch>

```sh
rm -rf ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git && \
mkdir -pv ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git

xpm link -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git
```

or

```sh
git -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git pull
```

## libucontext

- <https://github.com/xpack-3rd-party/libucontext-xpack>

```sh
rm -rf ~/Work/micro-os-plus-iii/libucontext-xpack.git && \
mkdir -pv ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/xpack-3rd-party/libucontext-xpack.git \
  ~/Work/micro-os-plus-iii/libucontext-xpack.git

xpm link -C ~/Work/micro-os-plus-iii/libucontext-xpack.git
```

```sh
git -C ~/Work/micro-os-plus-iii/libucontext-xpack.git pull
```

## cortexm

- <https://github.com/micro-os-plus/micro-os-plus-iii-cortexm>

```sh
rm -rf ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git && \
mkdor -pv ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii-cortexm.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git

xpm link -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git
```

```sh
git -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git pull
```

## chan-fatfs

- <https://github.com/xpacks/chan-fatfs>

```sh
rm -rf ~/Work/micro-os-plus-iii/chan-fatfs.git && \
mkdir -pv ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/xpacks/chan-fatfs.git \
  ~/Work/micro-os-plus-iii/chan-fatfs.git

xpm link -C ~/Work/micro-os-plus-iii/chan-fatfs.git
```

```sh
git -C ~/Work/micro-os-plus-iii/chan-fatfs.git pull
```
