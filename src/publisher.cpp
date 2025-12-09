#include <mqtt/async_client.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono;

int main(int argc, char** argv) {
    const std::string SERVER = "tcp://127.0.0.1:1883";
    const std::string CLIENT_ID = "local_pub_cpp";
    const std::string TOPIC = "test/local";
    int count = 1000;
    int interval_ms = 1; // message every 1 ms (adjust)

    if (argc > 1) count = std::stoi(argv[1]);
    if (argc > 2) interval_ms = std::stoi(argv[2]);

    mqtt::async_client cli(SERVER, CLIENT_ID);
    auto conn_tok = cli.connect();
    conn_tok->wait();

    for (int i = 0; i < count; ++i) {
        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        std::string payload = std::to_string(i) + "," + std::to_string(now);
        auto msg = mqtt::make_message(TOPIC, payload);
        msg->set_qos(1);
        // not retained
        cli.publish(msg);
        std::this_thread::sleep_for(milliseconds(interval_ms));
    }

    cli.disconnect()->wait();
    std::cout << "Publisher done\n";
    return 0;
}
