ESP32 WiFi & Bluetooth Tester — README (Chi tiết)
Mô tả ngắn

Project này là một sketch Arduino cho ESP32 để test và dùng thử các chức năng mạng không dây:

Tạo SoftAP (Access Point) và phục vụ Web UI tại http://192.168.4.1.

Quét Wi-Fi (SSID, RSSI, channel, encryption).

Kết nối ESP32 dưới chế độ STA (Station) đến Wi-Fi khác (thử kết nối).

Hiển thị số client kết nối AP.

Quét BLE (Bluetooth Low Energy) và trả về danh sách thiết bị.

Bluetooth Classic (SPP): start/stop SPP server, gửi/nhận tin nhắn.

Toggle LED và hiển thị thông tin thiết bị (MAC, IP).

Giao diện web đơn giản, responsive (fetch API).

Mục tiêu: một công cụ “mọi thứ trong một” cho việc kiểm tra Wi-Fi / BLE / BT Classic khi làm project với ESP32.

Nội dung gói (khi bạn tạo thư mục)

ESP32_Test_All.ino — file chính (sketch Arduino).

README.md hoặc README.txt — file README (nội dung dưới đây).

(Tùy chọn) ESP32_Test_All.zip — (nếu bạn nén thành zip để mở bằng Arduino IDE).

Yêu cầu phần mềm & phần cứng

Board: ESP32 (any dev board - ví dụ: ESP32 DevKitC, WROOM, WROVER...).

Arduino IDE: khuyên dùng phiên bản mới, ví dụ >= 1.8.19 (hoặc Arduino IDE 2.x).

ESP32 Arduino Core đã cài (Board Manager: esp32 by Espressif).

Thư viện: WiFi.h, WebServer.h, BluetoothSerial.h, BLEDevice.h — các thư viện này có sẵn trong ESP32 core. Không cần cài thêm.

Cáp microUSB/USB-C tương thích để nạp code.

Cấu hình (trong file .ino)

Ở đầu file .ino có phần CONFIG. Những biến bạn có thể chỉnh:

const char* ap_ssid = "ESP32_Test_AP";
const char* ap_pass = "esp32test"; // "" nếu muốn AP mở
const int LED_PIN = 2; // thay đổi nếu board khác


ap_ssid: tên Wi-Fi AP mà ESP32 phát.

ap_pass: password (để trống "" nếu muốn AP open).

LED_PIN: chân LED (thường là 2 trên nhiều dev board, nhưng có board khác dùng 5, 21...).

Nếu cần thay đổi thời gian scan BLE hoặc timeout khi connect STA, chỉnh trực tiếp trong các hàm (mình ghi chú rõ vị trí trong file .ino).

Hướng dẫn cài đặt & chạy (bước-bước)

Tạo file .ino:

Mở Notepad / VSCode / bất kỳ editor.

Dán toàn bộ code sketch (mà bạn đã có) vào.

Lưu file với tên ví dụ: ESP32_Test_All.ino (chỉ dùng chữ/số/gạch dưới; không dấu cách).

Mở Arduino IDE → File → Open → chọn file .ino.

Chọn board & COM port:

Tools → Board → chọn đúng loại ESP32 (ví dụ: "ESP32 Dev Module").

Tools → Port → chọn cổng COM của ESP32.

Upload:

Nhấn Upload. Mở Serial Monitor (115200) để xem log.

Sau khi nạp xong, ESP32 khởi động và phát AP.

Kết nối web UI:

Trên máy tính/điện thoại, connect Wi-Fi: SSID = ESP32_Test_AP, mật khẩu = esp32test (hoặc theo config).

Mở trình duyệt → vào http://192.168.4.1.

Giao diện sẽ hiện Device Info, Wi-Fi scan, BLE scan, BT Classic, LED control.

Web API (endpoints)

Web UI dùng các HTTP GET endpoints, bạn cũng có thể dùng curl để tương tác.

GET / — trả về trang HTML (web UI).

GET /status — trả về JSON:

{"mac":"AA:BB:CC:DD:EE:FF","ip":"192.168.4.1","led":true,"ap_clients":1}


GET /toggle — bật/tắt LED (trả về status như /status).

GET /scanwifi — scan Wi-Fi xung quanh, trả về mảng JSON: [{ "ssid": "...", "rssi": -50, "channel": 6, "enc": "WPA2_PSK" }, ...]

GET /connect_sta?ssid=XXX&pass=YYY — thử connect ESP32 đến Wi-Fi (STA) với SSID/Pass. Timeout mặc định 15s. Trả JSON {connected:true, ip:"192.168.x.x"} hoặc {connected:false,status:N}.

GET /stop_ap — dừng SoftAP (AP off).

GET /start_ap — start lại SoftAP.

GET /scan_ble — scan BLE trong ~5 giây, trả về mảng JSON: [{ "name":"", "address":"AA:BB:...", "rssi":-60 }, ...]

GET /bt_start — start Bluetooth Classic (SPP) server. ESP32 xuất hiện tên ESP32_SPP_Test.

GET /bt_stop — stop SPP.

GET /bt_send?msg=Hello — gửi dòng Hello qua SPP tới client (nếu đã connect).

Ví dụ curl
curl http://192.168.4.1/status
curl http://192.168.4.1/scanwifi
curl "http://192.168.4.1/connect_sta?ssid=HomeWiFi&pass=12345678"
curl http://192.168.4.1/scan_ble
curl http://192.168.4.1/bt_start
curl "http://192.168.4.1/bt_send?msg=Hello"

Giao diện Web — hướng dẫn dùng từng phần
Device

Hiển thị MAC (bạn có thể dùng để phân biệt nhiều board).

AP IP: thường 192.168.4.1.

LED: hiện đồ trạng thái, ấn "Toggle LED" để bật/tắt.

SoftAP clients: số lượng thiết bị đang kết nối AP.

Wi-Fi

Scan Wi-Fi: ấn nút, chờ kết quả — danh sách SSID, RSSI, encryption, channel.

Connect as STA: điền SSID + password → ấn Connect → device sẽ chuyển sang chế độ STA và thử kết nối.

Nếu kết nối thành công, bạn nhìn thấy IP assigned.

Lưu ý: khi ESP32 chuyển thành STA, SoftAP có thể bị ảnh hưởng (tùy mode), nên dùng start_ap để bật lại AP nếu cần.

AP Control: stop/start AP trực tiếp từ web.

BLE

Scan BLE (5s): quét xung quanh trong 5s, trả về tên/địa chỉ/RSSI.

Bluetooth Classic (SPP)

Start SPP: ESP32 hiện là thiết bị BT tên ESP32_SPP_Test. Trên điện thoại (Android), bạn có thể dò kết nối và pair nếu cần; dùng app terminal BT hoặc serial BT để kết nối.

Send Message: gửi tin nhắn tới client BT (ESP32 sẽ println() msg).

Wiring / Pinout

LED_PIN (mặc định 2): nếu board có LED tích hợp, dùng chân đó. Nếu dùng LED ngoài, nối:

LED anode (+) → GPIO (LED_PIN) qua resistor 220Ω

LED cathode (−) → GND

Nếu muốn thêm nhiều nút/chức năng, chỉnh code và thêm route + UI tương ứng.

Troubleshooting (Sự cố thường gặp & cách khắc phục)
1) Không thấy Wi-Fi AP (không thấy SSID)

Kiểm tra code đã upload thành công không (Serial Monitor).

Kiểm tra ap_ssid/ap_pass có chứa ký tự lạ không; đổi sang tên đơn giản.

Đảm bảo board có nguồn đủ (USB cáp tốt).

2) Không mở được http://192.168.4.1

Kiểm tra máy đã kết nối đúng AP của ESP32 không.

Một số điện thoại muốn “no internet” popup — bỏ qua và mở trình duyệt.

Dùng ping 192.168.4.1 (trên máy tính) để kiểm tra kết nối.

3) Scan Wi-Fi trả về mảng rỗng

Chờ vài giây và ấn Scan lại.

Đảm bảo lệnh scan không bị gọi liên tục (UI đã debounce).

4) connect_sta không thành công

Kiểm tra SSID/Password đúng.

Một số AP yêu cầu captive portal hoặc có MAC filter.

Kiểm tra wifi security (WPA2, WPA3, ...); ESP32 core có hạn chế với WPA3.

5) BLE scan không thấy thiết bị

Kiểm tra BLE device có đang broadcast không.

Tăng thời gian scan (thay start(5,false) thành start(8,false)).

Tắt/ mở lại BLE trên device mục tiêu.

6) Bluetooth Classic không connect được

Trên Android, khi SPP server bắt đầu, bạn cần pair / connect qua Bluetooth settings hoặc qua app terminal.

Một số điện thoại yêu cầu PIN; ESP32 SPP mặc định chấp nhận pairing nếu thiết bị client gửi pairing.

7) Lỗi compile: thiếu thư viện BLEDevice.h / BluetoothSerial.h

Cập nhật ESP32 board package bằng Boards Manager (esp32 by Espressif).

Mở Library Manager nếu cần (nhưng core đã có các lib này).

Kiểm tra & Debug

Serial Monitor (115200): xem log ở đầu chương trình (AP started, IP, errors).

Có thể chèn thêm Serial.println() tại các vị trí quan trọng (ví dụ: trước/sau scan, khi BT bắt đầu, khi connect STA).

Nếu muốn debug network stack chi tiết hơn, dùng esp_wifi API (nâng cao).

Mở rộng / Tối ưu (gợi ý cho dev)

Lưu cấu hình Wi-Fi: dùng SPIFFS, LittleFS hoặc Preferences để lưu SSID/password để tự kết nối lại.

Giao diện đẹp hơn: dùng Bootstrap hoặc một static index.html in SPIFFS để UI xịn hơn.

Token / Auth cho web UI: hiện UI open, nếu cần bảo mật, thêm basic auth hoặc form login.

Hiển thị chi tiết client AP: dùng ESP-IDF API esp_wifi_ap_get_sta_list() để lấy MAC client.

Revert về AP nếu STA fail: thiết kế fallback logic: nếu connect STA fail, bật lại AP tự động.

OTA (Over The Air): thêm endpoint để upload firmware mới qua web UI.

Graph RSSI/time-series: lưu RSSI qua thời gian rồi vẽ chart trên web page.

Lưu ý về bảo mật

AP mặc định có password esp32test — đổi trước khi dùng trong môi trường có rủi ro.

Web UI hiện không có authentication — tránh để chế độ production hoặc mạng công cộng.

Khi bật SPP, thiết bị sẽ hiển thị Bluetooth name — hạn chế gửi thông tin nhạy cảm.

FAQ (Câu hỏi thường gặp)

Q: Có thể chạy code này trên ESP32-S2 / S3 không?
A: Phần lớn tính năng Wi-Fi và BLE/BT Classic hỗ trợ, nhưng BLE/BT Classic support có thể khác giữa chip. ESP32-S2 không hỗ trợ Bluetooth Classic (SPP) — chỉ BLE. ESP32-S3 hỗ trợ cả hai thường tốt. Đảm bảo chọn đúng board trong Arduino IDE.

Q: Muốn thay đổi default AP IP (192.168.4.1)?
A: ESP32 softAP mặc định dùng subnet 192.168.4.x. Thay đổi cần dùng API WiFi.softAPConfig() trước khi softAP(); có thể thêm vào setup nếu cần:

IPAddress local_ip(192,168,10,1);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);
WiFi.softAPConfig(local_ip, gateway, subnet);
WiFi.softAP(ap_ssid, ap_pass);


Q: Làm sao để xem danh sách MAC client AP?
A: Cần dùng ESP-IDF API (native) hoặc esp_wifi_ap_get_sta_list() — không có API trực tiếp trong WiFi.h. Mình có thể bổ sung nếu bạn muốn.

Changelog (tóm tắt phiên bản)

v1.0 — Basic: AP + Web UI, Wi-Fi scan, BLE scan, BT Classic SPP, LED toggle.

v1.x — (tùy bạn thêm) sẽ bổ sung: lưu cấu hình, list client MAC, OTA.

Mã nguồn: ví dụ chỉnh nhanh (thay config)

Để đổi tên AP và password, sửa đầu file .ino:

const char* ap_ssid = "MyESP32AP";
const char* ap_pass = "mypass123";


Để tăng thời gian BLE scan (ví dụ 8 giây), tìm dòng:

BLEScanResults found = pBLEScan->start(5, false);


và đổi 5 thành 8.

Tạo file .ino tự động (tuỳ chọn)

Nếu bạn thích script, đây là ví dụ Python nhỏ (chạy trên máy tính) để tạo file .ino từ nội dung string (thay CODE_HERE bằng code sketch):

content = """<PASTE_YOUR_INO_CODE_HERE>"""
with open("ESP32_Test_All.ino", "w", encoding="utf-8") as f:
    f.write(content)
print("Wrote ESP32_Test_All.ino")

License

Bạn có thể sử dụng code này miễn phí cho mục đích học tập & thử nghiệm (MIT-like). Nếu muốn, mình có thể thêm header license MIT cụ thể vào đầu file .ino.
