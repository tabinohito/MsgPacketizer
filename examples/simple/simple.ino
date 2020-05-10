#include <MsgPacketizer.h>

// MsgPack::arr_t<T> = std::vector<T>
// MsgPack::map_t<T> = std::map<T>
// MsgPack::bin_t<T> = std::vector<T = uint8_t || char>

// input to msgpack
int i;
float f;
String s;
MsgPack::arr_t<int> v;
MsgPack::map_t<String, float> m;

const uint8_t recv_direct_index = 0x12;
const uint8_t send_direct_index = 0x34;
const uint8_t recv_lambda_index = 0x56;
const uint8_t send_back_index = 0x78;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    delay(2000);

    // update received data directly
    MsgPacketizer::subscribe(Serial, recv_direct_index, i, f, s, v, m);

    // handle received data with lambda which has incoming argument types/data
    MsgPacketizer::subscribe(Serial, recv_lambda_index,
    [&](const int& ii, const float& ff, const String& ss, const MsgPack::arr_t<int>& vv, const MsgPack::map_t<String, float>& mm)
    {
        // send data which is directly updated
        MsgPacketizer::send(Serial, send_direct_index, i, f, s, v, m);
        // send received data back
        MsgPacketizer::send(Serial, send_back_index, ii, ff, ss, vv, mm);
    });

    // callback which is always called when packet has come
    MsgPacketizer::subscribe(Serial, [&](const uint8_t index, MsgPack::Unpacker& unpacker)
    {
        static bool b = false;
        digitalWrite(LED_BUILTIN, b);
        b = !b;
    });
}

void loop()
{
    MsgPacketizer::parse(); // must be called to trigger callback
}
