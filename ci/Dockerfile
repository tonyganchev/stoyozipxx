FROM fedora

RUN dnf -y install cmake boost-devel boost-program-options g++ git make
RUN git clone https://github.com/tonyganchev/stoyozipxx.git /src
RUN cd /src \
	&& mkdir build \
	&& cd build \
	&& cmake .. \
	&& make -j
