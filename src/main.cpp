#include <swiftly/swiftly.h>
#include <swiftly/server.h>
#include <swiftly/database.h>
#include <swiftly/commands.h>
#include <swiftly/configuration.h>
#include <swiftly/logger.h>
#include <swiftly/timers.h>
#include <swiftly/gameevents.h>
#include <swiftly/menus.h>

Server *server = nullptr;
PlayerManager *g_playerManager = nullptr;
Database *db = nullptr;
Commands *commands = nullptr;
Configuration *config = nullptr;
Logger *logger = nullptr;
Timers *timers = nullptr;
Menus *menus = nullptr;

void OnProgramLoad(const char *pluginName, const char *mainFilePath)
{
    Swiftly_Setup(pluginName, mainFilePath);

    server = new Server();
    g_playerManager = new PlayerManager();
    commands = new Commands(pluginName);
    config = new Configuration();
    logger = new Logger(mainFilePath, pluginName);
    timers = new Timers();
    menus = new Menus(pluginName);
}

void LoadMenu()
{
    menus->UnregisterMenu("mvp");

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tempmap = {{"categories", {}}};
    for (uint32_t i = 0; i < config->FetchArraySize("swiftly_mvp.Sounds"); i++)
    {
        std::string category = config->Fetch<const char *>("swiftly_mvp.Sounds[%d].category", i);
        std::string name = config->Fetch<const char *>("swiftly_mvp.Sounds[%d].name", i);

        if (tempmap.find(category) == tempmap.end())
        {
            tempmap.insert({category, {}});
            tempmap["categories"].insert({std::string(config->Fetch<const char *>("swiftly_mvp.Categories.%s", category.c_str())), category});
        }
        tempmap[category].insert({name, std::string(format("sw_selectmvp %d", i))});
    }

    for (auto it = tempmap.begin(); it != tempmap.end(); ++it)
    {
        if (it->first == "categories")
            continue;
        menus->UnregisterMenu(it->first);
        menus->RegisterMenu(it->first, std::string(config->Fetch<const char *>("swiftly_mvp.Categories.%s", it->first.c_str())), "344CEB", it->second);
    }

    menus->RegisterMenu("mvp", "Custom MVP Menu", config->Fetch<const char *>("swiftly_mvp.ColorMenu"), tempmap["categories"]);
}

void OnPlayerSpawn(Player *player)
{
    if (!db->IsConnected())
        return;

    if (player->IsFirstSpawn() && !player->IsFakeClient())
        db->Query("insert ignore into `player_sounds` (steamid, song) values ('%llu', -1)", player->GetSteamID());
}

void OnRoundMVP(Player *player, short reason, long value, long musickitmvps, unsigned char nomusic, long musickitid)
{
    if (!db->IsConnected())
        return;

    int song = -1;
    int enable_sounds = 1;

    DB_Result result = db->Query("select * from `player_sounds` where steamid = '%llu' limit 1", player->GetSteamID());
    if (result.size() > 0)
        song = db->fetchValue<int>(result, 0, "song");
        enable_sounds = db->fetchValue<int>(result, 0, "enable_sounds");

    if (song != -1)
    {
    if (config->Exists("swiftly_mvp.Sounds[%d]", song))
    {
        std::string path = config->Fetch<const char *>("swiftly_mvp.Sounds[%d].path", song);
        std::string name = config->Fetch<const char *>("swiftly_mvp.Sounds[%d].name", song);
        if (enable_sounds == 1)
        {
            for (int i = 0; i < g_playerManager->GetPlayerCap(); i++)
            {
                Player *AllPlayers = g_playerManager->GetPlayer(i);
                if (!AllPlayers)
                    continue;

                AllPlayers->ExecuteCommand("play %s", path.c_str());
            }
            g_playerManager->SendMsg(HUD_PRINTCENTER, FetchTranslation("swiftly_mvp.PlayingMVP"), player->GetName(), name.c_str());
        } 
        else 
        {
            return;
        }
    }
    }
}

void Command_MVP(int playerID, const char **args, uint32_t argsCount, bool silent)
{
    if (playerID == -1)
        return;

    Player *player = g_playerManager->GetPlayer(playerID);

    if (player == nullptr)
        return;

    player->ShowMenu("mvp");
}

void Command_DisableSound(int playerID, const char **args, uint32_t argsCount, bool silent)
{

    if (playerID == -1)
        return;

    Player *player = g_playerManager->GetPlayer(playerID);

    if (player == nullptr)
        return;

    db->Query("update `player_sounds` set enable_sounds = '0' where steamid = '%llu' limit 1", player->GetSteamID());
    player->SendMsg(HUD_PRINTTALK, FetchTranslation("swiftly_mvp.SoundDisabled"));

}

void Command_EnableSound(int playerID, const char **args, uint32_t argsCount, bool silent)
{

    if (playerID == -1)
        return;

    Player *player = g_playerManager->GetPlayer(playerID);

    if (player == nullptr)
        return;

    db->Query("update `player_sounds` set enable_sounds = '1' where steamid = '%llu' limit 1", player->GetSteamID());
    player->SendMsg(HUD_PRINTTALK, FetchTranslation("swiftly_mvp.SoundEnabled"));

}

void Command_Volume(int playerID, const char **args, uint32_t argsCount, bool silent)
{
    if (playerID == -1)
        return;

    Player *player = g_playerManager->GetPlayer(playerID);

    if (player == nullptr)
        return;

    player->SetConvar("snd_toolvolume", "0.2");
}

void Command_SelectMVP(int playerID, const char **args, uint32_t argsCount, bool silent)
{
    if (playerID == -1)
        return;

    Player *player = g_playerManager->GetPlayer(playerID);

    if (player == nullptr)
        return;

    if (argsCount < 1)
    {
        player->SendMsg(HUD_PRINTTALK, FetchTranslation("swiftly_mvp.UsageSelectMVP"));
        return;
    }

    print("%s\n", args[0]);

    int soundid = StringToInt(args[0]);
    print("%d\n", soundid);

    if (soundid < 0)
        return;
    if (!config->Exists("swiftly_mvp.Sounds[%d]", soundid))
        return;

    print("%s\n", config->Fetch<const char *>("swiftly_mvp.Sounds[%d].name", soundid));

    db->Query("update `player_sounds` set song = '%d' where steamid = '%llu' limit 1", soundid, player->GetSteamID());
    player->SendMsg(HUD_PRINTTALK, FetchTranslation("swiftly_mvp.MVPSelected"), config->Fetch<const char *>("swiftly_mvp.Sounds[%d].name", soundid));
}

void OnPluginStart()
{

    db = new Database("swiftly_sounds");

    DB_Result result = db->Query("CREATE TABLE IF NOT EXISTS `player_sounds` (`steamid` varchar(128) NOT NULL, `song` int(11) NOT NULL, `enable_sounds` int(11) NOT NULL DEFAULT 0) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;");

    if (result.size() > 0)
        db->Query("ALTER TABLE `player_sounds` ADD UNIQUE KEY `steamid` (`steamid`);");

    commands->Register("selectmvp", reinterpret_cast<void *>(&Command_SelectMVP));
    commands->Register("mvp", reinterpret_cast<void *>(&Command_MVP));
    commands->Register("disablesound", reinterpret_cast<void *>(&Command_DisableSound));
    commands->Register("disablemvp", reinterpret_cast<void *>(&Command_DisableSound));
    commands->Register("enablesound", reinterpret_cast<void *>(&Command_EnableSound));
    commands->Register("enablemvp", reinterpret_cast<void *>(&Command_EnableSound));
    commands->Register("volume", reinterpret_cast<void *>(&Command_Volume));

    LoadMenu();
}

void OnPluginStop()
{
}

const char *GetPluginAuthor()
{
    return "";
}

const char *GetPluginVersion()
{
    return "1.0.0";
}

const char *GetPluginName()
{
    return "swiftly_mvp";
}

const char *GetPluginWebsite()
{
    return "";
}
