rm -rf pbgen
mkdir pbgen
protoc user.proto --cpp_out ./pbgen
