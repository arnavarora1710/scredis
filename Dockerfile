FROM --platform=linux/arm64 ubuntu:22.04

# Set non-interactive installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Set compiler flags for ARM NEON
ENV CXXFLAGS="-std=c++17 -Wall -Wextra"
ENV OPTFLAGS="-O3"
ENV ARCH="-march=armv8-a+simd"

# Copy source code and Makefile
COPY . /app/

# Set the default command to start a bash shell
CMD ["bash"]