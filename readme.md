# ZeroHelper

a local launcher of DDTank without install FlashPlayer

一个无需安装flash player就可以游玩弹弹堂的本地启动器

# Start 食用方法

## download 下载

 to release (in future)

去release 页面下载（正在搞鼓）

## build 构建

need mingw, vcpkg and qt6 with mingw version

需要本机环境有mingw, vcpkg和 mingw 版本的qt6

need GSL lib in vcpkg, use this command to install this lib.

需要再vcpkg里面安装gls库，使用下面的指令即可安装
```commandline

vcpkg install gsl:x64-windows

```

# Todo 待做

## add more services 需要添加更多的游戏服务器

it just only supports one server now, the 4399game server without captcha

现在仅支持4399大区的账号密码登录部分，且验证码待做

## auto take player avatar 自动获取角色头像

just screenshot and cut the avatar out

自动打开好友界面并截图取头像

## add CryptoJS to encrypt password for login 为登录添加加密函数

 now need the encrypt 7Road and 4399

4399和7道的登录方式需要附加加密函数防止第三方偷窥密码

## add webengine as backup way with login 添加浏览器登录模式

just use auto js take game url and load game

使用js完成登录和或许游戏凭证

# more...  更多

 coming sooooooon

咕咕咕
