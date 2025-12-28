
#pragma once

class MdrpcApplication {
public:
    virtual ~MdrpcApplication();
    static MdrpcApplication& GetInstance() {
        static MdrpcApplication app;
        return app;
    }
    static void Init(int argc, char** argv);

private:
    MdrpcApplication();
    MdrpcApplication(const MdrpcApplication&) = delete;
    MdrpcApplication(MdrpcApplication&&) = delete;
    MdrpcApplication& operator=(const MdrpcApplication&) = delete;
};
