FROM debian:bullseye

RUN echo "deb http://deb.debian.org/debian bullseye-backports main" | tee /etc/apt/sources.list.d/backports.list
RUN apt-get update
RUN apt-get install -y -t bullseye-backports qt6-base-dev qt6-base-dev-tools qt6-tools-dev-tools
RUN apt-get install -y build-essential cmake git
RUN apt-get install -y dcmtk
RUN apt-get install -y -t bullseye-backports libdcmtk-dev
RUN apt-get install -y x11-apps libx11-xcb1 libgl1-mesa-dev
RUN apt-get clean

RUN useradd -ms /bin/bash devuser
USER devuser
WORKDIR /home/devuser/app

CMD ["/bin/bash"]
