# API DOC

## connect interface

> client -> server

> server -> client

## photo yolo interface

> client -> server

```json
{
  "code": 1004,
  "sequence": "uuid",
  "devId": "xxx-xxx-xxx",
  "time": "timestamp",
  "model": "model",
  "game": "game",
  "msg": {
    "data": "xxxxxxxx"
  }
}
```

- code: 交互码
- sequence: uuid生成唯一标识码
- devId: 前三位为所属管理员编号，中间三位为设备所属区域，后三位为设备所属编号
- time：时间戳
- model: 寻求使用的模型
- game: 数据对应的游戏
- data: base64编码后的编码数据（图片进行base64编码）

> server -> client

```json
{
  "code": 1005,
  "result": 0,
  "sequence": "uuid",
  "devId": "xxx-xxx-xxx",
  "time": "timestamp",
  "model": "model",
  "game": "game",
  "msg": {
    "data": [["x", "y"]]
  }
}
```

- code: 交互码
- result: 返回值
- sequence: uuid生成唯一标识码
- devId: 前三位为所属管理员编号，中间三位为设备所属区域，后三位为设备所属编号
- time：时间戳
- model: 寻求使用的模型
- game: 数据对应的游戏
- data: 返回计算机出img图标



