#include "ParserNivel.h"

ParserNivel::ParserNivel()
{
    
    
}

ParserNivel::~ParserNivel()
{
    m_listaDeSprites.clear();
    m_escenario.listaDeElementos.clear();
    
    
}

void ParserNivel::clean()
{
    m_listaDeSprites.clear();
    m_escenario.listaDeElementos.clear();
    
}


bool ParserNivel::parsearDocumento(const std::string& nombreArchivoXML){
    
    bool exito = true;
    if (!parsearDoc(nombreArchivoXML, false))
    {
        std::stringstream ss;
        ss << "No se pudo parsear con éxito el archivo XML: " << nombreArchivoXML.c_str() << ".";
        Logger::Instance()->LOG(ss.str(), LogTypeError);
        ss.clear();
        ss << "Se cargará el archivo default: " << XML_NIVEL_DEFAULT_PATH << ".";
        Logger::Instance()->LOG(ss.str(), LogTypeWarn);
        
        exito = parsearDoc(XML_NIVEL_DEFAULT_PATH, true);
        if (!exito)
        {
            //El archivo XML default también tiene errores. No se pudo parsear.
            Logger::Instance()->LOG("No se pudieron parsear ninguno de los archivos XML nivel.", LogTypeError);
            //cout << "No se pudieron parsear ninguno delos archivos XML nivel";
        }
    }
    //cout << "Si se cargo \n";
    return exito;
    
}


bool ParserNivel::parsearDoc(const std::string& nombreArchivoXML, bool isDefault)
{
    bool parseadoExitoso = true;;
    std::stringstream ss;
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(nombreArchivoXML.c_str());
    
    if (!result)
    {
        parseadoExitoso = false;
        //No se pudo abrir el archivo XML
        //LOGUEO DE ERRORES EN CASO DE QUE NO SE PUEDA CARGAR EL ARCHIVO XML
        ss << "Archivo " << nombreArchivoXML.c_str() <<  " dañado. Error Description: " << result.description() << ".";
        Logger::Instance()->LOG(ss.str(), LogTypeError);
        
        return parseadoExitoso;
    }
    
    
    extraerSprites(&doc);
    extraerVentana(&doc,0);
    extraerEscenario(&doc,0);
    extraerAvion(&doc,0);
    return parseadoExitoso;
}

bool ParserNivel::extraerSprites(const pugi::xml_document* doc)
{
    bool exito = true;
    pugi::xml_node spritesNode = doc->child("sprites");
    
    for (pugi::xml_node spr = spritesNode.first_child(); spr; spr = spr.next_sibling())
    {
        Sprite sprite;
        std::string id = spr.child("id").first_child().value();
        std::string path = spr.child("path").first_child().value();
        std::string cantidad = spr.child("cantidad").first_child().value();
        std::string ancho = spr.child("ancho").first_child().value();
        std::string alto = spr.child("alto").first_child().value();
        if (!validarNumero(cantidad))
        {
            Logger::Instance()->LOG("Se cargo el valor cantidad del sprite por default", LogTypeWarn);
            cantidad = "2";
        }
        if (!validarNumero(ancho))
        {
            Logger::Instance()->LOG("Se cargo el valor ancho del sprite por default", LogTypeWarn);
            ancho = "11";
        }
        if (!validarNumero(alto))
        {
            Logger::Instance()->LOG("Se cargo el valor alto del por default", LogTypeWarn);
            alto = "11";
        }
        if (!validarPath(path))
        {
            Logger::Instance()->LOG("Se cargo el path del sprite por default", LogTypeWarn);
            path = "Assets/Sprites/missing.jpg";
            //id = "default";
            
        }
        
        sprite.id = id;
        sprite.path = path;
        sprite.cantidad = stoi(cantidad);
        sprite.ancho = stoi(ancho) ;
        sprite.alto = stoi(alto);
        m_listaDeSprites.push_back(sprite);
    }
    return exito;
}

bool ParserNivel::extraerVentana(const pugi::xml_document* doc, bool isLoadingDefault)
{
    bool exito = true;
    pugi::xml_node ventanaNode = doc->child("ventana");
    std::string anchoString = ventanaNode.child("ancho").first_child().value();
    std::string altoString = ventanaNode.child("alto").first_child().value();
    
    if (!validarNumero(anchoString))
    {
        Logger::Instance()->LOG("Se cargo el ancho de la ventana por default", LogTypeWarn);
        anchoString = "810";
    }
    if (!validarNumero(altoString))
    {
        Logger::Instance()->LOG("Se cargo el alto de la ventana por default", LogTypeWarn);
        altoString = "610";
    }
    
    m_ventana.alto = stoi(altoString);
    m_ventana.ancho = stoi(anchoString);
    return true;
    
    
}


bool ParserNivel::extraerEscenario(const pugi::xml_document* doc, bool isLoadingDefault)
{
    
    bool exito = true;
    pugi::xml_node escenarioNode = doc->child("escenario");
    std::string cantStagesStr = escenarioNode.child("cantidadStages").first_child().value();
    std::string cantJugadoresStr = escenarioNode.child("cantidadMaximaJugadores").first_child().value();
    
    pugi::xml_node gameModes = escenarioNode.child("GameModes");
    int gameMode = atoi(gameModes.child("GameMode").first_child().value());
    pugi::xml_node gameTeams = gameModes.child("GameModeTeams");
    std::vector<GameTeam> teamList;
    
    for (pugi::xml_node team = gameTeams.first_child(); team; team = team.next_sibling()) {
        int teamID = atoi(team.child("TeamID").first_child().value());
        std::string teamName = team.child("TeamName").first_child().value();
        
        GameTeam newGameTeam;
        newGameTeam.gameTeamID = teamID;
        newGameTeam.gameTeamName = teamName;
        newGameTeam.gameTeamScore = 0;
        
        teamList.push_back(newGameTeam);
    }

    if (gameMode == 0) {
        this->m_escenario.gameMode = GameModeCooperative;
    } else {
        this->m_escenario.gameMode = GameModeCompetition;
    }
    
    this->m_escenario.teamsList = teamList;

    if (!validarNumero(cantJugadoresStr))
    {
        Logger::Instance()->LOG("Se cargo la cantidad de jugadores del escenario por default", LogTypeWarn);
        cantJugadoresStr = "2";
    }
    
    std::string anchoString = escenarioNode.child("ancho").first_child().value();
    
    if (!validarNumero(anchoString))
    {
        Logger::Instance()->LOG("Se cargo el ancho del escenario por default", LogTypeWarn);
        anchoString = "110";
    }
    std::string altoString = escenarioNode.child("alto").first_child().value();
    if (!validarNumero(altoString))
    {
        Logger::Instance()->LOG("Se cargo el alto del escenario por default", LogTypeWarn);
        altoString = "1100";
    }
    std::string velScrollString = escenarioNode.child("velocidadScroll").first_child().value();
    if (!validarNumero(velScrollString))
    {
        Logger::Instance()->LOG("Se cargo velocidad de scrolling del escenario por default", LogTypeWarn);
        velScrollString = "5";
    }
    
    extraerFondo(doc,1);
    extraerElementos(doc,1);
    m_escenario.cantidadJugadores = std::stoi(cantJugadoresStr);
    m_escenario.cantidadStages = std::stoi(cantStagesStr);
    m_escenario.ancho = std::stoi(anchoString);
    m_escenario.alto = std::stoi(altoString);
    m_escenario.velScroll = std::stoi(velScrollString);
    m_escenario.fondo = m_fondo;
    m_escenario.listaDeElementos = m_listaDeElementos;
    return exito;
    
    
}
bool ParserNivel::extraerElementos(const pugi::xml_document* doc, bool isLoadingDefault)
{
    bool exito = true;
    pugi::xml_node elemNode = doc->child("escenario");
    elemNode = elemNode.child("elementos");
    for (pugi::xml_node eleme = elemNode.first_child(); eleme; eleme = eleme.next_sibling())
    {
        Elemento elem;
        std::string id = eleme.child("spriteId").first_child().value();
        
        bool found = false;
        std::vector<Sprite> lista =  getListaSprites();
        for(int i=0; i < lista.size(); i++)
        {
            if (id == lista[i].id)
            {
                found = true;
            }
        }
        if(found)
        {
            elem.spriteId = id;
        }
        else
        {
            Logger::Instance()->LOG("Al no tener un id existente entre los sprites, se asignará un sprte default", LogTypeWarn);
            elem.spriteId = "default";
        }
        std::string posx = eleme.child("posicion").child("x").first_child().value();
        std::string posy = eleme.child("posicion").child("y").first_child().value();
        Posicion pos;
        if (!validarNumero(posx))
        {
            Logger::Instance()->LOG("Se cargo la posx del elemento por default", LogTypeWarn);
            posx = "1";
        }
        if (!validarNumero(posy))
        {
            Logger::Instance()->LOG("Se cargo la posy del elemento por default", LogTypeWarn);
            posy = "1";
        }
        pos.x = stoi(posx);
        pos.y = stoi(posy);
        elem.posicion = pos;
        
        m_listaDeElementos.push_back(elem);
    }
    return exito;
}

bool ParserNivel::extraerFondo(const pugi::xml_document* doc, bool isLoadingDefault)
{
    bool exito = true;
    pugi::xml_node escenarioNode = doc->child("escenario");
    std::string anchoString = escenarioNode.child("fondo").child("ancho").first_child().value();
    std::string altoString = escenarioNode.child("fondo").child("alto").first_child().value();
    std::string spriteIdString = escenarioNode.child("fondo").child("spriteId").first_child().value();
    
    if(! validarNumero(anchoString))
    {
        Logger::Instance()->LOG("Se cargo el ancho del fondo por default", LogTypeWarn);
        anchoString = "12";
    }
    if(! validarNumero(altoString))
    {
        Logger::Instance()->LOG("Se cargo el alto del fondo por default", LogTypeWarn);
        altoString = "12";
    }
    
    m_fondo.ancho = std::stoi(anchoString);
    m_fondo.alto = std::stoi(altoString);
    m_fondo.spriteId = spriteIdString;
    
    return exito;
    
}

bool ParserNivel::extraerAvion(const pugi::xml_document* doc, bool isLoadingDefault)
{
    bool exito = true;
    pugi::xml_node avionNode = doc->child("avion");
    std::string velDesplStr = avionNode.child("velocidadDesplazamiento").first_child().value();
    std::string velDispStr = avionNode.child("velocidadDisparos").first_child().value();
    std::string cdDispStr = avionNode.child("cooldownDisparos").first_child().value();
    std::string avionSprId = avionNode.child("avionSpriteId").first_child().value();
    std::string vueltaSprId = avionNode.child("vueltaSpriteId").first_child().value();
    std::string disparpSprId = avionNode.child("disparosSpriteId").first_child().value();
    
    if(! validarNumero(velDesplStr))
    {
        Logger::Instance()->LOG("Se cargo la velocidad del avion por default", LogTypeWarn);
        velDesplStr = "10";
    }
    if(! validarNumero(velDispStr))
    {
        Logger::Instance()->LOG("Se cargo la velocidad del disparo por default", LogTypeWarn);
        velDispStr = "12";
    }
    if(! validarNumero(cdDispStr))
    {
        Logger::Instance()->LOG("Se cargo el cd del disparo por default", LogTypeWarn);
        cdDispStr = "300";
    }
    bool found = false;
    std::vector<Sprite> lista =  getListaSprites();
    for(int i=0; i < lista.size(); i++)
    {
        if (avionSprId == lista[i].id)
        {
            found = true;
        }
    }
    if(!found)
    {
        Logger::Instance()->LOG("Al no tener un id existente entre los sprites, se asignará un sprte default", LogTypeWarn);
        avionSprId = "default";
    }
    found = false;
    std::vector<Sprite> lista2 =  getListaSprites();
    for(int i=0; i < lista2.size(); i++)
    {
        if (vueltaSprId == lista2[i].id)
        {
            found = true;
        }
    }
    if(!found)
    {
        Logger::Instance()->LOG("Al no tener un id existente entre los sprites, se asignará un sprte default", LogTypeWarn);
        vueltaSprId = "default";
    }
    found = false;
    std::vector<Sprite> lista3 =  getListaSprites();
    for(int i=0; i < lista3.size(); i++)
    {
        if (disparpSprId == lista3[i].id)
        {
            found = true;
        }
    }
    if(!found)
    {
        Logger::Instance()->LOG("Al no tener un id existente entre los sprites, se asignará un sprte default", LogTypeWarn);
        disparpSprId = "default";
    }
    
    m_avion.velDespl = std::stoi(velDesplStr);
    m_avion.velDisp = std::stoi(velDispStr) + m_avion.velDespl;
    m_avion.cdDisp = std::stoi(cdDispStr);
    m_avion.avionSprId = avionSprId;
    m_avion.vueltaSprId = vueltaSprId;
    m_avion.disparoSprId = disparpSprId;
    
    return exito;
}


bool ParserNivel::is_file_exist(std::string& fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}
bool ParserNivel::validarPath(std::string& path)
{
    return is_file_exist(path);
}

bool validarRoot(const pugi::xml_document* doc)
{
    return false;
}

bool validarSprite(Sprite* sprite)
{
    return false;
}

bool validarVentana(Ventana* ventana)
{
    return false;
}

bool validarEscenario(Escenario *escenario)
{
    return false;
}

bool ParserNivel::validarNumero(std::string& intString)
{
    if (intString.empty())
        return false;
    quitarEspacios(intString);
    for (uint i = 0 ; i < intString.length(); i++)
    {
        if (!std::isdigit(intString[i]))
            return false;
    }
    return true;
}


bool validarTipoDeDato(std::string& tipoDatoString)
{
    return false;
}
