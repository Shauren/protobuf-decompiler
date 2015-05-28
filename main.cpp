
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <fstream>

int main(int argc, char* argv[])
{
    google::protobuf::DescriptorPool pool;
    pool.AllowUnknownDependencies();
    
    std::string proto = "\n\017reverseme.proto\022\025com.example.reverseme"
    "\"r\n\010SomeData\0227\n\004data\030\001 \002(\0132).com.example"
    ".reverseme.SomeData.OtherData\022\017\n\007comment"
    "\030\002 \001(\t\032\034\n\tOtherData\022\017\n\004test\030\001 \002(\005:\0011"; 
    
    google::protobuf::FileDescriptorProto fileDescProto;
    if (fileDescProto.ParseFromString(proto))
    {
        if (google::protobuf::FileDescriptor const* fileDesc = pool.BuildFile(fileDescProto))
        {
            std::ofstream f(fileDesc->name());
            f << fileDesc->DebugString() << std::endl;
            f.close();
        }
    }
    
    return 0;
}
