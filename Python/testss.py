from bs4 import BeautifulSoup
import json
if __name__ == "__main__":
    soup = BeautifulSoup(
        open("C:/Phoenix/Practice/Python/ss.html", 'r', encoding='UTF-8'), "lxml")
    gui_configs = []  # 储存服务器配置列表
    str_server = {}  # 储存单个服务器配置
    PATH = "C:/Phoenix/sandboxie/ERISED/drive/C/Program Files (x86)/Shadowsocks/gui-config.json"
    for target_list in soup.findAll("tr"):
        str = target_list.text.split('\n')
        str_server['server'] = str[2]
        str_server['server_port'] = str[3]
        str_server['password'] = str[4]
        str_server['method'] = str[5]
        str_server['plugin'] = ""
        str_server['plugin_opts'] = ""
        str_server['plugin_args'] = ""
        str_server['remarks'] = str[7]
        str_server['timeout'] = 5
        gui_configs.append(str_server)
        str_server={}
        pass
    gui_config = {
        "version": "4.1.7.1",
        "configs": gui_configs,
        "strategy": 'null',
        "index": 0,
        "global": 'false',
        "enabled": 'false',
        "shareOverLan": 'false',
        "isDefault": 'false',
        "isIPv6Enabled": 'false',
        "localPort": 1080,
        "portableMode": 'true',
        "pacUrl": 'null',
        "useOnlinePac": 'false',
        "secureLocalPac": 'true',
        "availabilityStatistics": 'false',
        "autoCheckUpdate": 'true',
        "checkPreRelease": 'false',
        "isVerboseLogging": 'false',
        "logViewer": {
            "topMost": 'false',
            "wrapText": 'false',
            "toolbarShown": 'false',
            "Font": "Consolas, 8pt",
            "BackgroundColor": "Black",
            "TextColor": "White"
        },
        "proxy": {
            "useProxy": 'false',
            "proxyType": 0,
            "proxyServer": "",
            "proxyPort": 0,
            "proxyTimeout": 3,
            "useAuth": 'false',
            "authUser": "",
            "authPwd": ""
        },
        "hotkey": {
            "SwitchSystemProxy": "",
            "SwitchSystemProxyMode": "",
            "SwitchAllowLan": "",
            "ShowLogs": "",
            "ServerMoveUp": "",
            "ServerMoveDown": "",
            "RegHotkeysAtStartup": 'false'
        }
    }

    try:
        result = json.dumps(gui_config)
        with open(PATH, "w+", encoding="utf-8") as f:
            f.write(result)
    except Exception:
        print(">>  Generate gui-config.json failed")
    else:
        print(">>  Generate gui-config.json successful")
