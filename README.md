# Influx DB

Die nachfolgende Dokumentation basiert auf einer Projektarbeit für das Modul 242 an der Technischen Berufsschule Zürich, von Yves Wetter, Ricardo Bras und Davide Logiurato.

## 1. Beschreibung
InfluxDB ist ein hochleistungsfähiger Datenspeicher, der im Besonderen für Zeitreihendaten erfunden wurde. Er erlaubt Datenerfassung mit hohem Durchsatz sowohl Komprimierung und Echtzeitabfragen der erfassten Informationen. InfluxDB kommt in allen Anwendungsfällen als Datenspeicher zum Einsatz, binnen denen enorme Mengen von Fakten mit Zeitstempel verarbeitet werden, exemplarisch DevOps-Überwachungsdaten, Protokolldaten, Anwendungsmetriken, Fakten von IoT-Sensoren oder Echtzeitanalysedaten. Der Datenspeicher stellt genauso alternative Charakteristika disponibel, außerdem Datenaufbewahrungsrichtlinien. InfluxDB Enterprise offeriert Hochverfügbarkeit ebenso eine hochgradig erweiterbare Clustering-Lösung für Bereiche der Applikation mit Zeitreihendaten.

Deshalb ist es perfekt für IOT-Geräte, wie zum Beispiel Sensoren.

### 1.1 InfluxDB vs. SQL
InfluxDB ist für die Arbeit mit Zeitreihendaten konzipiert. SQL-Datenbanken können mit Zeitreihen arbeiten, wurden aber nicht ausschliesslich zu diesem Zweck erstellt. Kurz gesagt, InfluxDB ist dafür ausgelegt, eine grosse Menge an Zeitreihendaten zu speichern und eine schnelle Echtzeitanalyse dieser Daten durchzuführen.

## 2. Installation
1. git klonen
2. vagrant deploymend (cd /M242-IOT/lb02/InfluxDB && vagrant up)
3. main.cpp anpassen (TrustStore, URL, API-Key)
4. mbed kompilieren
5. Deploy kompiliertes File auf das IOT-kit-v3
6. IOT-Kit-v3 neu starten
7. Installation fertig
8. Datenbank auslesen.

## 3. Dokumentation
### 3.1 Passwörter
Die Passwörter werden bei jeder Vagrant Installation generiert. das Passwort ist immer 20 Zeichen lang. Somit ist es sehr sicher gegen Bruteforce Attacken.

Die Passwörter können nach der Installation hier gefunden werden:
```
./install/InfluxDB_(iotkit,superuser).txt
```

### 3.2 Datenbanken
Es wurde eine Datenbank mit dem Namen "m242" erstellt. Die Tabellen werden automatisch mit dem Schicken der API-Requests erstellt. 
Grundsätzlich verwenden wir in diesem Projekt die Tabelle lb02. In diese werden folgende Daten geschrieben:

  | Feld-Name      | Beschreibung                                                                                                                         |
  | -------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
  | host           | Hier kann man beim IOT-Kit einen Hostnamen festlegen. Damit kann sichergestellt werden, welches Board diese Messdaten geschickt hat. |
  | location       | Dieses Feld wird mit einem Standort befüllt. Idealerweise werdne GPS-Daten vom Board verwendet.                                      |
  | in_temperature | Daten des Temperatursensor des Boards.                                                                                               |
  | ex_temperature | Daten des externen Temperatursensor.                                                                                                 |
  | in_humidity    | Daten des internen Luftfeuchtigkeit Sensor.                                                                                          |
  | ex_humidity    | Daten des externen Luftfeuchtigkeit Sensor.                                                                                          |

### 3.3 TLS / SSL
Die Daten werden via TLS,SSL an die Datenbank versendet.

Das SSL-Zertifikat wird bei der Installation automatisch generiert. Die Details des Zertifikats sehen so aus:
'/C=CH/ST=private/L=private/O=Zuerich/CN=<IP-des-Interfaces>'
Die Zertifikate sind hier auf dem Server abgelegt:
```
/etc/ssl/influxdb-selfsigned.crt
/etc/ssl/influxdb-selfsigned.key
```

### 3.4 Command Line Interface (CLI)
Um Daten auszulesen, muss man sich zuerst anmelden. Dazu muss man folgende Schritte machen:
1. ssh Verbindung mit dem DB-Server herstellen.
2. ausführen.
```
sudo influx -precision rfc3339 -unsafeSsl -ssl -username "superuser" -password "123456789"
```
3. Datenbank auswählen.
```
use m242
```
4. Eine Query ausführen.
```
SELECT * FROM "lb02"
```

### 3.5 Application Programming Interface (API)
Die InfluxDB kann über mehrere API angesprochen werden. Sie unterstützt zum Beispiel ein Rest API.

Diese kann man zum Beispiel mit Curl simulieren. Grundsätzlich kann somit jeder Webclient eines IOT-Boards damit Kommunizieren.

```
curl -i -k -XPOST "https://192.168.1.166:8086/write?db=m242&u=iotkit&p=1111222223333" --data-binary 'lb02,host=yves-iot,location=genf in_temperature=91,ex_temperature=99,in_humidity=50,ex_humidity=10'
```
Wenn man die Daten mit einer Webrequest machen will, dann muss man diese als Json übermitteln. Das Zertifikat ist selfsigned, deshalb muss man unsichere Zertifikate akzeptieren.

### 3.6 Multi Device Pattform
Unsere InfluxDB kann Daten von mehreren Geräten Daten empfangen und kann diese richtig in die Datenbank schreiben. Um die Daten der einzenlen Boards zu unterscheiden werden diese zwei Werte mitgeschickt.
- Gerätename
- Standort (Manuell, oder GPS)

## 4. Testprotokoll
### 4.1 Vagrant installation
| Testfall: 001     | Vagrant installation                                              |
| ----------------- | ----------------------------------------------------------------- |
| Ziel:             | Der InfluxDB Server kann mit Vagrant installiert werden.          |
| Beschreibung:     | Das git clonen und mit vagrant probieren die Influxdb zu deployen |
| Soll-Wert:        | vagrant wurde installiert und ist erreichbar.                     |
| Ist-Wert:         | installation ging ohne Problem.                                   |
| Analyse:          | alles in Ordnung.                                                 |
| Weitere Schritte: | -                                                                 |

### 4.2 Vagrant SSL
| Testfall: 002     | Vagrant SSL                                                        |
| ----------------- | ------------------------------------------------------------------ |
| Ziel:             | Vagrant deployed eine sichere Umgebung.                            |
| Beschreibung:     | Das git clonen und mit vagrant probieren die Influxdb zu deployen. |
| Soll-Wert:        | Auf die Influxdb kann nur über SSL zugegriffen werden.             |
| Ist-Wert:         | Es funktioniert nur ein zugriff über SSL.F                         |
| Analyse:          | alles in Ordnung.                                                  |
| Weitere Schritte: | -                                                                  |

### 4.3 Influxdb security
| Testfall: 003     | Influxdb security                                                          |
| ----------------- | -------------------------------------------------------------------------- |
| Ziel:             | Die Influxdb kann sicher deployed werden und wird auch sicher konfiguriert |
| Beschreibung:     | Das Deployen mehrmahls ausführen und die Passwörter vergeichen             |
| Soll-Wert:        | Die Passwörter sind bei jeder installation unterschiedlich.                |
| Ist-Wert:         | Die Passwörter sind unterschiedlich.                                       |
| Analyse:          | alles in Ordnung.                                                          |
| Weitere Schritte: | -                                                                          |

### 4.4 Influxdb Berechtigungskonzept
| Testfall: 004     | Influxdb Berechtigungskonzept                                                           |
| ----------------- | --------------------------------------------------------------------------------------- |
| Ziel:             | Die Berechtigungen des Clouddienst ist nach dem least Privilage prinzip gemacht worden. |
| Beschreibung:     | Der IOT user hat nur schreibrechte. Der Adminuser hat alle Rechte.                      |
| Soll-Wert:        | IOT hat nur Schreibrechte. Der Adminuser hat alle Rechte.                               |
| Ist-Wert:         | Das Berechtigungskonzept stimmt.                                                        |
| Analyse:          | alles in Ordnung.                                                                       |
| Weitere Schritte: | -                                                                                       |

### 4.5 Mbed kompilieren
| Testfall: 005     | Mbed kompilieren                                                                                                                          |
| ----------------- | ----------------------------------------------------------------------------------------------------------------------------------------- |
| Ziel:             | Die Source kann kompiliert werden. Es gibt keine Fehler, wenn man auf das Programm auf das Board lädt.                                    |
| Beschreibung:     | in das Verzeichnis Source wechseln. Danach kann man mit dem Mbed-CLI die source compilen. Alle depencencys sind bereits in diesem Ordner. |
| Soll-Wert:        | Es gab keine Fehlermeldungen                                                                                                              |
| Ist-Wert:         | Das Kompilieren hat funktioniert                                                                                                          |
| Analyse:          | alles in Ordnung.                                                                                                                         |
| Weitere Schritte: | -                                                                                                                                         |

### 4.6 IOT-kitV3 Serial Verbindung
| Testfall: 006     | IOT-kitV3 Serial Verbindung                                                |
| ----------------- | -------------------------------------------------------------------------- |
| Ziel:             | Die Seriell Verbindung kann als debug gebraucht werden.                    |
| Beschreibung:     | Mit Putty auf die Com3 9600 zugreiffen und schauen, was das Board ausgibt. |
| Soll-Wert:        | Die IP-Addresse und der Verbindungsaufbau wird ausgegeben.                 |
| Ist-Wert:         | Alles wird gelogt.                                                         |
| Analyse:          | alles in Ordnung.                                                          |
| Weitere Schritte: | -                                                                          |

### 4.7 Daten auslesen
| Testfall: 007     | Daten auslesen                                                                   |
| ----------------- | -------------------------------------------------------------------------------- |
| Ziel:             | Die Daten werden an den Influxdb Server verschickt und können ausgelesen werden. |
| Beschreibung:     | Sich als Admin auf der Influxdb anmelden und ein Select * from M242.lb02 machen. |
| Soll-Wert:        | Es werden Korrekte Daten ausgegeben.                                             |
| Ist-Wert:         | Es werden Daten an den Server verschickt. Diese können wieder ausgelesen werden. |
| Analyse:          | alles in Ordnung.                                                                |
| Weitere Schritte: | -                                                                                |

### 4.8 Multi Device Plattform
| Testfall: 008     | Multi Device Plattform                                                                                                                |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| Ziel:             | Der Cloud-Dienst ist darauf ausgelegt, dass mehrere IOT-Devices Daten an die InfluxDB verschicken können.                             |
| Beschreibung:     | Da sich der Standort und der Gerätename mit in die Datenbank geschrieben wird, kann die InfluxDB für mehrere geräte gebraucht werden. |
| Soll-Wert:        | Der Standort und der Gerätename wird an die InfluxDB mitgeschickt.                                                                    |
| Ist-Wert:         | Der Gerätename und der Standort wird mitgeschickt und in die Datenbank geschrieben.                                                   |
| Analyse:          | alles in Ordnung.                                                                                                                     |
| Weitere Schritte: | -                                                                                                                                     |

## 4.9 Datensicherheit bei der Übermittlung
| Testfall: 009     | Datensicherheit bei der Übermittlung                                                                                                                                                                               |
| ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| Ziel:             | Die Daten werden sicher mit der InfluxDB ausgetauscht. Die Daten werden als Json geschickt und sind somit dank SSL verschlüsselt. Die URL ist auch verschlüsselt und kann mit einem Proxy nicht mitgelesen werden. |
| Beschreibung:     | Die Daten können nicht von Man-In-The-Midle mitgelesen werden. Der austausch ist verschlüsselt. Wenn man einen Proxy dazwischen stellt, kann er keine Passwörter und Daten mitlesen.                               |
| Soll-Wert:        | Passwörter können nicht mitgelesen werden.                                                                                                                                                                         |
| Ist-Wert:         | Der Datenaustausch ist sicher.                                                                                                                                                                                     |
| Analyse:          | alles in Ordnung.                                                                                                                                                                                                  |
| Weitere Schritte: | -                                                                                                                                                                                                                  |

## 4.10 SSL-breaker-Proxy (Datensicherheit)
| Testfall: 010     | Datensicherheit bei der Übermittlung                                                                              |
| ----------------- | ----------------------------------------------------------------------------------------------------------------- |
| Ziel:             | Ein Proxy, welcher den SSL-Traffik wird nicht unterstützt, da dieser nicht im SSL-Trust vom Programm ist.         |
| Beschreibung:     | Wenn man ein IOT-Device in ein Netzwerk steckt, welches einen SSL-Breaker-Proxy hat kann keine Daten verschicken. |
| Soll-Wert:        | Der SSL-Trust wurde unterbrochen. Deshalb werden keine Daten an die InfluxDB verschickt.                          |
| Ist-Wert:         | Der Datenaustausch ist sicher. Ein SSL-breaker wird geblockt, da dieser nicht im Truststore ist.                  |
| Analyse:          | Alles funktioniert wie geplant.                                                                                                 |
| Weitere Schritte: | -                                                                                                                 |