FROM gcc:10.2.0

WORKDIR /home

RUN git clone https://github.com/cpputest/cpputest.git && \
    cd cpputest && \
    autoreconf . -i && \
    ./configure && \
    make install 

ENV CPPUTEST_HOME="/home/cpputest"

WORKDIR /home/src