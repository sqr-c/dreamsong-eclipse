cmake -B build/ -D BUILD=i && \
make -C build/ && \
sudo make -C build/ install && \
echo "Success!"
