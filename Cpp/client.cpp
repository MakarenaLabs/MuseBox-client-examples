#include <zmq.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
typedef unsigned char BYTE;

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64(unsigned char c)
{
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(BYTE const *buf, unsigned int bufLen)
{
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--)
  {
    char_array_3[i++] = *(buf++);
    if (i == 3)
    {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }

  return ret;
}

int main(int argc, char *argv[])
{
  // Check args
  if (argc < 3)
  {
    std::cout << "Usage of client demo: ./client topic image|txt_file [image2]" << std::endl;
    return -1;
  }

  // Check topic
  std::vector<std::string> valid_topics{"FaceDetection", "FaceRecognition", "FaceLandmark", "AgeDetection", "GenderDetection", "GlassesDetection", "EmotionRecognition",
                                        "TextDetection", "LogoDetection", "LogoRecognition", "PeopleDetection", "ObjectDetection", "MonoDepth", "MonoDepth2", "MedicalDetection", "MedicalSegmentation",
                                        "HumanSegmentation", "ImagePortrait", "StochasticDifference", "ZeroCrossingRate", "SignalEnergy", "EnergyEntropy", "FFT", "STFT", "Monoaudio2Midi"};
  // For face recognition the database is located on /usr/local/bin/database/people
  // For logo recognition the database is located on /usr/local/bin/database/logos

  if (!(std::find(std::begin(valid_topics), std::end(valid_topics), argv[1]) != valid_topics.end()))
  {
    std::cout << "The topic is not valid." << std::endl;
    std::cout << "Valid topics: FaceDetection, FaceRecognition, FaceLandmark, EyeBlink, "
                 "AgeDetection, GenderDetection, GlassesDetection, EmotionRecognition, TextDetection, LogoDetection, "
                 "LogoRecognition, PeopleDetection, ObjectDetection, MonoDepth, MonoDepth2, MedicalDetection, MedicalSegmentation, HumanSegmentation, ImagePortrait, StochasticDifference"
              << std::endl;
    return -1;
  }

  // set the ZMQ context
  void *context = zmq_ctx_new();
  void *publisher = zmq_socket(context, ZMQ_PUB);
  assert(publisher);
  // bind to the queue as publisher
  int bind = zmq_bind(publisher, "tcp://127.0.0.1:9696");
  if (bind < 0)
  {
    unsigned int error_code = zmq_errno();
    printf("zmq_bind server ctx error: %u, %s\n", error_code, zmq_strerror(error_code));
    assert(bind == 0);
  }
  nlohmann::json messageToSend;

  std::string fn = argv[2];
  if (fn.substr(fn.find_last_of(".") + 1) == "txt")
  {
    std::ifstream in;
    in.open(fn);
    messageToSend = {
        {"clientId", "1"},                  // client ID
        {"topic", argv[1]},                 // Machine Learning task
        {"publisherQueue", "tcp://*:5556"}, // where the MuseBox subscriber will respond
        {"input", nlohmann::json::array()}, // image to infer
    };

    if (in.is_open())
    {
      int i = 0;
      double element = 0.0;
      while (in >> element)
      {
        // std::cout << element << std::endl;
        messageToSend["input"].push_back(element);
      }
    }

    in.close();
  }

  else
  {
    // create a cv::Mat that represents the image
    cv::Mat img = cv::imread(argv[2]);
    cv::resize(img, img, cv::Size(640, 360));

    // create the vector that "flats" the cv::Mat in a single dimension
    std::vector<uchar> img_buf;
    cv::imencode(".jpg", img, img_buf);
    auto *enc_msg = reinterpret_cast<unsigned char *>(img_buf.data());
    std::string encoded = base64_encode(enc_msg, img_buf.size());

    messageToSend = {
        {"clientId", "1"},                  // client ID
        {"topic", argv[1]},                 // Machine Learning task
        {"publisherQueue", "tcp://*:5556"}, // where the MuseBox subscriber will respond
        {"image", encoded},                 // image to infer
    };
  }

  if (std::string(argv[1]) == "StochasticDifference")
  {
    cv::Mat img2 = cv::imread(argv[3]);
    cv::resize(img2, img2, cv::Size(640, 360));

    // create the vector that "flats" the cv::Mat in a single dimension
    std::vector<uchar> img_buf2;
    cv::imencode(".jpg", img2, img_buf2);
    auto *enc_msg2 = reinterpret_cast<unsigned char *>(img_buf2.data());
    std::string encoded2 = base64_encode(enc_msg2, img_buf2.size());

    messageToSend["image2"] = encoded2;
  }

  // send the message
  std::cout << messageToSend.dump().c_str() << std::endl;
  int output = zmq_send(publisher, messageToSend.dump().c_str(), messageToSend.dump().size(), 0);
  if (output < 0)
  {
    unsigned int error_code = zmq_errno();
    printf("server ctx error: %u, %s\n", error_code, zmq_strerror(error_code));
    assert(output == 0);
  }

  std::cout << "sent " << std::endl;

  // open a socket for receive the response
  void *socket = zmq_socket(context, ZMQ_SUB);
  int connect = zmq_connect(socket, "tcp://127.0.0.1:5556");
  int rc = zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
  long length = 2400000;
  char *buf = new char[length];
  std::fill_n(buf, length, 0);
  std::cout << "######################## Waiting for server ################################" << std::endl;

  // blocking receive for ZMQ
  int nbytes = zmq_recv(socket, buf, length, 0);
  std::string str_msg = buf;

  // parse the json response (from string to object)
  auto message = nlohmann::json::parse(str_msg);

  std::cout << message << std::endl;

  return 0;
}
