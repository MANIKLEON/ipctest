#include <mqtt/async_client.h>
#include <chrono>
#include <iostream>
#include <string>
#include <atomic>

using namespace std::chrono;

class callback : public virtual mqtt::callback {
public:
    std::atomic<int> received{0};
    void connected(const std::string& cause) override {
        std::cout << "[sub] connected\n";
    }
    void connection_lost(const std::string& cause) override {
        std::cerr << "[sub] lost connection: " << cause << "\n";
    }
    void message_arrived(mqtt::const_message_ptr msg) override {
        auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        std::string payload = msg->to_string();
        // Expect payload "seq,ts"
        auto comma = payload.find(',');
        if (comma != std::string::npos) {
            int seq = std::stoi(payload.substr(0, comma));
            long long pub_ts = std::stoll(payload.substr(comma + 1));
            long long rtt = now - pub_ts;
            std::cout << "msg " << seq << " rtt_ms=" << rtt << "\n";
        } else {
            std::cout << "msg: " << payload << "\n";
        }
        received++;
    }
};

int main() {
    const std::string SERVER = "tcp://127.0.0.1:1883";
    const std::string CLIENT_ID = "local_sub_cpp";
    const std::string TOPIC = "test/local";

    mqtt::async_client cli(SERVER, CLIENT_ID);
    callback cb;
    cli.set_callback(cb);
    cli.connect()->wait();
    cli.start_consuming();
    cli.subscribe(TOPIC, 1)->wait();

    std::cout << "Subscriber listening. Press Ctrl+C to stop.\n";
    while (true) std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
