**Dependencies**  
Tests requires python librosa to be installed on your system's python.   
`sudo pip install librosa`

Default build system uses ffmpeg to compute ffts. That library requires nasm to build
`brew install nasm`

**Build**  
Build system uses cmake. It's only tested on macos for now  
`mkdir build`  
`cd build`  
`cmake -G Xcode ..`
