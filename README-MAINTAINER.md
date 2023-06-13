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


## posix-arch

- <https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch>

```sh
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git

xpm link -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-posix-arch.git
```

## libucontext

- <https://github.com/xpack-3rd-party/libucontext-xpack>

```sh
git clone \
  --branch xpack-develop \
  https://github.com/xpack-3rd-party/libucontext-xpack.git \
  ~/Work/micro-os-plus-iii/libucontext-xpack.git

xpm link -C ~/Work/micro-os-plus-iii/libucontext-xpack.git
```

## cortexm

- <https://github.com/micro-os-plus/micro-os-plus-iii-cortexm>

```sh
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii-cortexm.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git

xpm link -C ~/Work/micro-os-plus-iii/micro-os-plus-iii-cortexm.git
```

## chan-fatfs

- <https://github.com/xpacks/chan-fatfs>

```sh
git clone \
  --branch xpack-develop \
  https://github.com/xpacks/chan-fatfs.git \
  ~/Work/micro-os-plus-iii/chan-fatfs.git

xpm link -C ~/Work/micro-os-plus-iii/chan-fatfs.git
```
