#!/usr/bin/env python
# -*- coding: utf8 -*-
from threading import Timer
import webapp2, jinja2, os, random, objects, time

class MyTimer:
    def __init__(self):
        self.timer = Timer(60, None)

    def getTime(self):
        return self.timer.

    def resetTimer(self):
        self.timer = Timer(60, None)

    def startTimer(self):
        self.timer.start()

class Players:
    def __init__(self, nom, squad):
        self.nom = nom              # nom du participant
        self.dead = None            # Etat du joueur (mort ou vif)
        self.messages = []          # file des messages recus en attente de lecture
        self.squad = squad          # Equipe du joueur
        self.state = 1              #statut du joueur 1 = vivant 0 = mort
        self.nbKill = 0
        self.nbDeath = 0

    def getCoords(self):
        if self.latitude <> None or self.longitude <> None :
            return str(self.latitude)+", "+str(self.longitude)
        else :
            return "None"

    def setDead(self):
        self.nbDeath += 1
        self.setAlive(0)

    def setRespawn(self):
        self.setAlive(1)

    def addKill(self):
        self.nbKill += 1

    def getSquad(self):
        return self.squad

    def setAlive(self, state):
        self.state = state

    def isAlive(self):
        if self.state == 0:
            return False
        if self.state == 1:
            return True

    def setSquad(self, numSquad):
        self.squad = numSquad

    def setMessage(self, msg):
        self.messages.append(msg)

    def getMessage(self):
        if len(self.messages) <> 0 :
            save = self.messages[0]
            self.messages = self.messages[1:]
            return save
        else :
            return "None"

class Objects :
    def __init__(self, **kwargs):
        self.nom = kwargs['nom']
        self.prop = kwargs['prop']

    def getNom(self):
        return self.nom

    def getProp(self):
        return self.prop

class Coords:
    def __init__(self, **kwargs):
        self.x = kwargs['x']
        self.y = kwargs['y']

    def getX(self):
        return self.x

    def getY(self):
        return self.y


class HideandSeek:

    def __init__(self, mapNumber = 1, actif = True):
        self.enCours = False                # booleen
        self.players = {}                   # dictionnaire de cles nom et valeurs Participant
        self.mapNumber = mapNumber          # Numéro de la map à jouer
        self.objects = []                   # liste des objets
        self.coordsMap = []                 # coordonnées de la map (un carré)
        self.vainqueur = None               # vainqueur du HideandSeek lorsque connu
        self.messageBox = []                # messages à toute la partie
        self.actif = actif                  # état du serveur
        self.host = ""                    # Nom de l'hote
        self.numberOfPlayers = 0         # Nombre de joueurs total, en vie, morts, par équipes
        self.numberOfPlayerSeeker = 0
        self.numberOfDeadPlayerSeeker = 0
        self.numberOfPlayerHider = 0
        self.numberOfDeadPlayerHider = 0
        self.timer = MyTimer()

    def startGame(self):
        self.timer.resetTimer()

    def reinitGame(self):
        self.enCours = True
        self.players = {}
        self.numberOfPlayerSeeker = 0
        self.numberOfDeadPlayerSeeker = 0
        self.numberOfPlayerHider = 0
        self.numberOfDeadPlayerHider = 0
        self.loadObj()
        self.loadMap()
        self.vainqueur = None

    def stopGame(self):
        self.enCours = False

    def loadMap(self):
        if self.mapNumber == 1:
            coordsArray = objects.coordsMap1
            for coords in coordsArray:
                self.objects.append(Coords(**coords))

    def getCoordsMap(self):
        message = ""
        for coords in self.coordsMap :
            message += coords + ","
        return message

    def loadObj(self):
        if self.mapNumber == 1:
            objectsArray = objects.objectsMap1
            for object in objectsArray:
                self.objects.append(Objects(**object))

    def getObjs(self):
        message = ""
        for obj in self.objects:
            message += obj+","

    def addPlayer(self, cle):
        if cle not in self.players :
            if self.numberOfPlayers == 0 or self.numberOfPlayers == None:
                self.host = cle
            if self.numberOfPlayerSeeker == self.numberOfPlayerHider:
                participant = Players(cle, 1)
                self.players[cle] = participant
                self.numberOfPlayerHider += 1
            else :
                participant = Players(cle, 2)
                self.players[cle] = participant
                self.numberOfPlayerSeeker += 1
            self.numberOfPlayers +=1

    def removePlayer(self, cle):
        if cle in self.players :
            participant = self.getPlayer(cle)
            if participant.getSquad() == 1 :
                self.numberOfPlayerHider -= 1
            else :
                self.numberOfPlayerSeeker -= 1
            del self.players[cle]
            self.numberOfPlayers -= 1

    def getPlayer(self, cle):
        return self.players[cle]

    def setWinner(self, winner):
        self.vainqueur = winner

    def setPlayerKilledBy(self, player, killedBy):
        participant = self.getPlayer(player)
        if participant.getSquad() == 1 :
            self.numberOfDeadPlayerHider += 1
        else :
            self.numberOfDeadPlayerSeeker += 1
        participant.setDead()
        if self.numberOfPlayerHider == self.numberOfDeadPlayerHider:
            self.setWinner("Seekers")

    def isPlayerAlive(self, cle):
        participant = self.getPlayer(cle)
        return participant.isAlive()



class ServeurManager(webapp2.RequestHandler):
    nbServeur = 0 #nombre de serveurs actifs
    serveurs = {}

    def addServeur(self, cle, serveur):
        self.serveurs[cle] = serveur
    def removeServeur(self, cle):
        del ServeurManager.serveurs[cle]

    def getServeur(self, cle):
        return HideandSeek.players[cle]

    def get(self):
        jinja_environnement = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))
        template_values = {
        }
        self.response.headers['Content-Type'] = 'text/html'
       #try :
        serveurNumber = self.request.get('serveur')
        serveurAction = self.request.get('act')
        if serveurNumber != None :
            currentGame = None
            if serveurNumber in self.serveurs.keys() :
                currentGame = self.serveurs[serveurNumber]
            else :
                hideAndSeekGame = HideandSeek()
                self.addServeur(serveurNumber, hideAndSeekGame)
                hideAndSeekGame.actif = True
                currentGame = hideAndSeekGame
            if serveurAction == 'getServeurs':
                message = ""
                for serveur in self.serveurs:
                    message = message + "," +serveur
                self.response.write(message)
            cmd = self.request.get('cmd')
            playerName = self.request.get('name').encode()
            squad = self.request.get('squad')
            message = self.request.get('msg').encode()
            killedby = self.request.get('killedby').encode()
            if cmd == 'isServeurEnable':
                self.response.write(currentGame.enCours)
            elif cmd == 'startGame':
                currentGame.startGame()
                self.response.write("OK")
            elif cmd == 'reinitGame':
                currentGame.reinitGame()
                self.response.write("OK")
            elif cmd == 'stopGame':
                currentGame.stopGame()
                self.response.write("OK")
            elif cmd == 'addPlayer':
                currentGame.addPlayer(playerName)
                self.response.write("OK")
            elif cmd == 'removePlayer':
                currentGame.removePlayer(playerName)
                self.response.write("OK")
            elif cmd == 'getPlayers':
                message = ""
                for player in currentGame.players:
                    message = message + "," + player
                message = message[1:]
                self.response.write(message)
            elif cmd == 'getSquad':
                squad = currentGame.getPlayer(playerName).getSquad()
                self.response.write(squad)
            elif cmd == 'setSquad':
                currentGame.players[playerName].setSquad(squad)
                self.response.write("OK")
            elif cmd == 'sendMessage':
                currentGame.players[playerName].setMessage(message)
                self.response.write("OK")
            elif cmd == 'getMessage':
                self.response.write(currentGame.players[playerName].getMessage())
            elif cmd == 'getWinner':
                self.response.write(currentGame.vainqueur)
            elif cmd == 'startBeginTimer':
                currentGame.timer.startTimer()
                self.response.write("OK")
            elif cmd == 'getBeginTimerValue':
                self.response.write(currentGame.timer.getTime())
        #except:
         #   self.response.write(None)


app = webapp2.WSGIApplication([
    ('/', ServeurManager)
], debug=True)


