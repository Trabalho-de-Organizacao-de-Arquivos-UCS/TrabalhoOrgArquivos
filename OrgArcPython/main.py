from ctypes import *
from pymongo import *
from cryptography.fernet import Fernet
import pprint
import pymongo
import webbrowser

class MongoDb():
     db = MongoClient('localhost', 27017)

MongoConection = MongoDb()



class TMovie:
    Name = ""
    Hashtags = []
    Count =0
    def __init__(self,Name,Hashtags):
        self.Name = Name
        self.Hashtags = Hashtags
        self.Count = 0

    def Name(self):
        return self.Name

    def Hashtags(self):
        return self.Hashtags

    def Count(self):
        return self.Count

    def NewCount(self,Count):
        self.Count = Count



class Index(Structure):
    _fields_ = [('hashtag', c_char * 200),
                ('pos_in_file', c_long)]

class Twitter(Structure):
    _fields_ = [('id_twitter', c_int),
                ('usuario', c_char * 20),
                ('mensagem', c_char * 280),
                ('data', c_char * 8),
                ('pais', c_char * 20),
                ('hashtag', c_char * 200)]


def InsertDBFromFile():
    TwitterDB = MongoConection.db.dbo.Twitter



    with open('/data/college/TrabalhoOrgArquivos/file.dat', 'rb') as file:
        TwittsList = []
        x = Twitter()
        while file.readinto(x) == sizeof(x):
            twitt = {
                "id_twitter": x.id_twitter,
                "usuario": x.usuario.decode('iso-8859-1'),
                "mensagem": x.mensagem.decode('iso-8859-1'),
                "data": x.data.decode('iso-8859-1'),
                "pais": x.pais.decode('iso-8859-1'),
                "hashtag": FernetEncrypt(x.hashtag.decode('iso-8859-1'))
            }
            print("Writing: " + x.hashtag.decode('iso-8859-1'))
            TwittsList.append(twitt)
        TwitterDB.insert_many(TwittsList)

def TDDImportationSucessful():
    TwitterDB = MongoConection.db.dbo.Twitter
    count = TwitterDB.count()

    print('\nA importação deve importar 147732 registros')
    print('TDD Funcinou? ' + str(count == 147732))
    print('Registros retornados: ' +  str(count))

def MongoHashtagIndex():
    TwitterDB = MongoConection.db.dbo.Twitter
    TwitterDB.create_index([('hashtag', pymongo.ASCENDING)], unique = True)
    print(sorted(list(TwitterDB.index_information())))


def MongoUserIndex():
    TwitterDB = MongoConection.db.dbo.Twitter
    TwitterDB.create_index([('usuario', pymongo.ASCENDING)], unique = False)
    print(sorted(list(TwitterDB.index_information())))

def TDDHashtagsMustBeStoragedWithCrypto():
    TwitterDB = MongoConection.db.dbo.Twitter

    for tweet in TwitterDB.find().limit(1):
        print('Hashtag armazenada: ' + str(tweet["hashtag"]))
        print('Hashtag descriptografada: ' + FernetDecrypt(tweet["hashtag"]))

def SelectAll():
    TwitterDB = MongoConection.db.dbo.Twitter
    for Twitter in TwitterDB.find():
        Twitter["hashtag"] = FernetDecrypt(Twitter["hashtag"])
        pprint.pprint(Twitter)

def SelectHashtag(hashtag, printit = True):
    if printit: print("Conectando...")
    TwitterDB = MongoConection.db.dbo.Twitter

    if printit: print("Reunindo informações...")
    compare = hashtag.replace('#','')
    compare = compare.lower()
    if printit: print(compare)

    count = 0
    for Twitter in TwitterDB.find():
        dbhashtag = FernetDecrypt(Twitter["hashtag"]).lower()
        if(compare in dbhashtag):
            count += 1
            if printit:
                Twitter["hashtag"] = dbhashtag
                print(Twitter)

    if printit: print("Hashtags encontrados: " + str(count))
    return count

def SelectUser(user, printit = True):
    if printit: print("Conectando...")
    TwitterDB = MongoConection.db.dbo.Twitter
    if printit: print("Reunindo informações...")
    compare = user.lower()
    if printit: print(compare)

    count = 0
    for Twitter in TwitterDB.find():
        dbuser = Twitter["usuario"].lower()
        if(compare == dbuser):
            count += 1
            if printit:
                dbhashtag = FernetDecrypt(Twitter["hashtag"]).lower()
                Twitter["hashtag"] = dbhashtag
                pprint.pprint(Twitter)

    if printit: print("Usuários encontrados: " + str(count))
    return count

def TDDSelectHashtagMustWorkAndBeCaseUnsensitive():
    searchparam1 = 'inktober2019'
    searchparam2 = 'INKtoBER2019'

    count1 = SelectHashtag(searchparam1, False)
    count2 = SelectHashtag(searchparam2, False)

    print('\nA busca de hashtag \"' + searchparam1 + '\" deve retornar 545 registros')
    print('TDD Funcinou? ' + str(count1 == 545))
    print('Registros retornados: ' +  str(count1))

    print('\nA busca deve não diferenciar maiusculas e minusculas entao a busca por \"' + searchparam2 + '\" deve retornar o mesmo número de registros que por "' + searchparam1 + '\"')
    print('TDD Funcinou? ' + str(count1 == count2))
    print('Registros retornados: ' +  str(count2))

def TDDSelectUserMustWorkAndBeCaseUnsensitive():
    searchparam1 = 'Reylo4Ever'
    searchparam2 = searchparam1.lower()

    count1 = SelectUser(searchparam1, False)
    count2 = SelectUser(searchparam2, False)

    print('A busca do usuário \"' + searchparam1 + '\" deve retornar 73 registros')
    print('TDD Funcinou? ' + str(count1 == 73))
    print('Registros retornados: ' +  str(count1))

    print('A busca deve não diferenciar maiusculas e minusculas entao usuário \"' + searchparam2 + '\" deve retornar o mesmo número de registros que do usuário ' + searchparam1)
    print('TDD Funcinou? ' + str(count1 == count2))
    print('Registros retornados: ' +  str(count2))
    

def LoadtTwitterList():
    TwitterDB = MongoConection.db.dbo.Twitter
    List = []
    for Twitter in TwitterDB.find():
        Twitter["hashtag"] = FernetDecrypt(Twitter["hashtag"])
        List.append(Twitter)
    return List

def setMovies(Name,Hashtags):
    movie = TMovie(Name,Hashtags)
    return movie


def getCount(Name,Hashtags,TList):
    Count = 0;
    ##my_list = filter(lambda x: x.attribute == value, my_list)
    for hash in Hashtags:
        matches = list(filter(lambda x: hash in x["hashtag"], TList))
        matchesCount = len(matches)
        Count = Count + matchesCount

    movie = TMovie(Name,Hashtags)
    movie.NewCount(Count)
    return movie




def MovieCounts():
    print("Carregando Dados...")
    TList = LoadtTwitterList()
    print("Dados Carregados! {} registros ".format(len(TList)))

    ListMovies = []
    MaxCount = 0

    Joker = ["Joker", "Coringa"]
    Rambo = ["Rambo", "RamboLastBlood", "RamboAtéOFim"]
    Terminator = ["Terminator", "TerminatorDarkFate", "OExterminadorDoFuturo", "OExterminadordoFuturoDestinoSombrio"]
    StarWars = ["StarWars", "StarWarsTheRiseOfSkywalker", "StarWarsAAscensãoSkywalker", "#StarWars2019"]
    ZombielandTwo = ["Zumbilândia2", "Zombieland2"]
    ProjectGemini = ["ProjectGemini", "ProjetoGemini"]
    AdAstra = ["AdAstra"]

    print("Realizando Contagens...")
    tJoKer = getCount("Joker",Joker,TList)
    MaxCount = MaxCount + tJoKer.Count
    ListMovies.append(tJoKer)

    tRambo= getCount("Rambo", Rambo, TList)
    MaxCount = MaxCount + tRambo.Count
    ListMovies.append(tRambo)

    tTerminator = getCount("Terminator", Terminator, TList)
    MaxCount = MaxCount + tTerminator.Count
    ListMovies.append(tTerminator)

    tStarWars= getCount("StarWars", StarWars, TList)
    MaxCount = MaxCount + tStarWars.Count
    ListMovies.append(tStarWars)

    tZombielandTwo= getCount("ZombielandTwo", ZombielandTwo, TList)
    MaxCount = MaxCount + tZombielandTwo.Count
    ListMovies.append(tZombielandTwo)

    tProjectGemini = getCount("ProjectGemini", ProjectGemini, TList)
    MaxCount = MaxCount + tProjectGemini.Count
    ListMovies.append(tProjectGemini)

    tAdAstra = getCount("AdAstra", AdAstra, TList)
    MaxCount = MaxCount + tAdAstra.Count
    ListMovies.append(tAdAstra)
    print("Calculando %...")
    data_donut = []
    data_bar = []
    for movie in ListMovies:
        perc = (movie.Count * 100) / MaxCount
        print("Filme: {0} Total: {1} - {2:.2f}%".format(movie.Name,movie.Count,perc))
        data_donut.append({"value": round(perc,2), "label": movie.Name})
        data_bar.append({"y": movie.Count, "x": movie.Name})

    f = open('data.jsonp', 'w')

    content = "json_donut = {0};json_donut = JSON.stringify(json_donut);json_bar = {1};json_bar = JSON.stringify(json_bar);".format(data_donut,data_bar)

    f.write(content)
    f.close()

    filename = 'file:///C:/Users/Gabri/Documents/Projetos/TrabalhoOrgArquivos/OrgArcPython/index.html'
    webbrowser.open_new_tab(filename)










class Encrpt:
    def __init__(self, token, key):
        self.token = token
        self.key = key

    def token(self):
        return self.token

    def key(self):
        return self.key


def FernetGenerateKey():
    key = Fernet.generate_key()
    file = open("/data/college/TrabalhoOrgArquivos/key.bin", "wb")
    file.write(key)

def GetFernetKey():
    file = open("/data/college/TrabalhoOrgArquivos/key.bin", "rb")
    key = file.read()
    return key

def FernetEncrypt(hashtag):
    key = GetFernetKey()
    f = Fernet(key)
    token = f.encrypt(hashtag.encode('iso-8859-1'))
    return token

def FernetDecrypt(token):
    key = GetFernetKey()
    f = Fernet(key)
    a = f.decrypt(token)
    return a.decode('iso-8859-1')

def menu():

    menu = "\nMenu principal:\n\t1- Listar todos os Registros\n\t2- Pesquisar Hashtag\n\t3- Responder Hipótese\n\t4- Inserir Dados do Arquivo no Banco\n\t5- Pesquisar Usuario"
    menutdd = "\nTDDs:\n\t10 - Quantidade tweets importados\n\t11 - Busca de hashtag\n\t12 - Busca de usuario\n\t13 - Armazenamento cryptografado\n"
    sair = False

    while sair != True:
        print(menu)
        print(menutdd)
        opcao = int(input("Digite uma opção: "))
        if(opcao == 1):
            SelectAll()
        elif(opcao == 2):
            hashtag = input("Informe a hashtag: ")
            SelectHashtag(hashtag)
        elif(opcao == 3):
            MovieCounts()
        elif(opcao == 4):
            InsertDBFromFile()
        elif(opcao == 5):
            user = input("Informe o usuario: ")
            SelectUser(user) #test with Reylo4Ever
        elif(opcao == 10):
            TDDImportationSucessful()
        elif(opcao == 11):
            TDDSelectHashtagMustWorkAndBeCaseUnsensitive()
        elif(opcao == 12):
            TDDSelectUserMustWorkAndBeCaseUnsensitive()
        elif(opcao == 13):
            TDDHashtagsMustBeStoragedWithCrypto()
        elif (opcao == 0):
            sair = True




def main():

    menu()
     ##InsertDBFromFile() INSERIR NO BANCO





main()


