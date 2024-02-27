<p align="center">
  <a href="https://github.com/swiftly-solution/swiftly_mvp">
    <img src="https://cdn.swiftlycs2.net/swiftly-logo.png" alt="SwiftlyLogo" width="80" height="80">
  </a>

  <h3 align="center">[Swiftly] MVP</h3>

  <p align="center">
    A simple plugin for Swiftly that implements an Custom MVP system.
    <br/>
  </p>
</p>

<p align="center">
  <img src="https://img.shields.io/github/downloads/swiftly-solution/swiftly_mvp/total" alt="Downloads"> 
  <img src="https://img.shields.io/github/contributors/swiftly-solution/swiftly_mvp?color=dark-green" alt="Contributors">
  <img src="https://img.shields.io/github/issues/swiftly-solution/swiftly_mvp" alt="Issues">
  <img src="https://img.shields.io/github/license/swiftly-solution/swiftly_mvp" alt="License">
</p>

---

### Installation 👀

1. Download the newest [release](https://github.com/swiftly-solution/swiftly_mvp/releases).
2. Everything is drag & drop, so i think you can do it!
3. Setup database connection in `addons/swiftly/configs/databases.json` with the key `swiftly_sounds` like in the following example:
```json
{
    "swiftly_sounds": {
        "host": "...",
        "username": "...",
        "password": "...",
        "database": "...",
        "port": 3306
    }
}
```
(!) Don't forget to replace the `...` with the actual values !!

### Configuring the plugin 🧐

* After installing the plugin, you will need to configure your addon pack, you can setup the addon from ``configs/addons.json``
* We prepared for the plugin as a base pack an addon pack with around 50 MVP's, the config is already configured with it, you will just need to add this id: ``3167176839`` in ``configs/addons.json`` and drag and drop the config from the release archive into ``configs/plugins``
* We will make a tutorial soon on how you can make your own addon sound pack.

### TODO 🛡️

* Make more default addon packs with different music genres.
* Add translations for messages in chat.
* Find a method to change the volume of the MVP.

### Commands 🤖
* !disablesound (Disables the MVP's sounds)
* !enablesound (Enables back the MVP's sounds)

### Creating A Pull Request 😃

1. Fork the Project
2. Create your Feature Branch
3. Commit your Changes
4. Push to the Branch
5. Open a Pull Request

### Have ideas/Found bugs? 💡
Join [Swiftly Discord Server](https://swiftlycs2.net/discord) and send a message in the topic from `📕╎plugins-sharing` of this plugin!

---