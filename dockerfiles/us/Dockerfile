FROM debian:10 as us-build

RUN apt-get update && apt-get install -y \
  openssh-server \
  supervisor \
  curl \
  vim \
  iptables \
  figlet \
  libssl-dev \
  libncurses5-dev \
  libncurses5 \
  libcurl4-gnutls-dev \
  apt-utils \
  htop \
  lsof \
  net-tools \
  autotools-dev \
  gnupg \
  bash-completion \
  automake \
  m4 \
  libtool \
  pkg-config

# ----------------------------- libarchive -----------------------------

RUN cd /opt && \
  curl -LO https://github.com/libarchive/libarchive/archive/v3.3.1.tar.gz
RUN cd /opt && \
  tar -xvf ./v3.3.1.tar.gz
RUN rm -rvf /opt/v3.3.1.tar.gz
RUN cd /opt/libarchive-3.3.1 && /bin/sh ./build/autogen.sh && \
  ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var && \
  make -j 4 && \
  make install
# ----------------------------------------------------------------------

# ----------------------------- libyaml -----------------------------
RUN cd /opt && \
  curl -LO https://github.com/yaml/libyaml/archive/0.2.2.tar.gz
RUN cd /opt && \
  tar -xvf ./0.2.2.tar.gz
RUN rm -rvf /opt/0.2.2.tar.gz
RUN cd /opt/0.2.2 && \
  ./bootstrap && ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var && \
  make -j 4 && \
  make install
# -------------------------------------------------------------------

WORKDIR /usr/local/wscu

COPY ./include/ ./include
COPY ./src/ ./src

RUN cd ./src && \
	make


ENTRYPOINT ["us-build"]
