rm -rf pbgen
mkdir pbgen
protoc rpcheader.proto --cpp_out ./pbgen
