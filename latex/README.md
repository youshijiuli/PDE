# latexify 的使用方法
latexify 是一个将 python 代码转化为 latex 代码的工具。

> 为了不用写繁杂的latex代码，将代码转化为latex代码，方便使用。

## 安装
```
pip install latexify
```

## 使用
### 1. 基本使用
```
from latexify import latexify   
latexify(data, enviroment="align*")
```

### 2. 参数说明     
#### 2.1 data
data 可以是列表，元组，字典，集合，字符串，数值，布尔值，None 等。

#### 2.2 enviroment
enviroment 表示 latex 代码的格式，默认为 align*，表示对齐环境。     
#### 2.3 示例
```
from latexify import latexify
latexify([1, 2, 3], enviroment="align*")
```                 