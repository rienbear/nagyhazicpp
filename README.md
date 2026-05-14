import markdown

readme_content = """# Malom Játék (Mill Game) - C++ Projekt

Ez a projekt egy klasszikus, konzolos felületű malomjáték implementációja C++ nyelven. A program lehetővé teszi két játékos számára, hogy egymás ellen játsszanak, kezeli a játékfázisokat, ellenőrzi a szabályok betartását, valamint támogatja a játékállás mentését és betöltését.

## Főbb Jellemzők

* **Teljes körű szabályrendszer:** Kezeli a bábu lerakást, mozgatást és a speciális "ugrás" szabályt (amikor egy játékosnak már csak 3 köve marad).
* **Malom detektálás:** Automatikusan felismeri a kialakult malmokat és lehetővé teszi az ellenfél kövének eltávolítását.
* **Mentés és Betöltés:** A játék állapota minden lépés után menthető a `mentes.txt` fájlba, és indításkor visszatölthető.
* **Konzolos felület:** Egyszerű, karakteres megjelenítés a hordozhatóság érdekében.
* **Beépített egységtesztek:** A `gtest_lite.h` keretrendszer segítségével ellenőrizhető a logika helyessége.
* **STL-mentes megvalósítás:** A feladat előírásai szerint a projekt nem használ STL tárolókat, helyettük dinamikus memóriakezelést alkalmaz.

## Projekt Felépítése

A program objektumorientált szemléletben készült, az alábbi osztályhierarchiával:

* **`Jatek`**: A játékmenet fő koordinátora. Kezeli a játékosok váltását, a fázisokat és a fájlkezelést.
* **`JatekTabla`**: A 24 mezőből álló tábla reprezentációja. Tárolja a kövek helyzetét, a szomszédsági kapcsolatokat és ellenőrzi a malmokat.
* **`Jatekos`**: Absztrakt alaposztály a játékosok számára.
    * **`FeherJatekos`**: A fehér kövekkel (W) játszó fél implementációja.
    * **`FeketeJatekos`**: A fekete kövekkel (B) játszó fél implementációja.

## Fordítás és Futtatás

A program szabványos C++11 vagy újabb fordítóval fordítható.

### Fordítás parancssorból:
```bash
g++ -o malom main.cpp jatek.cpp jatekos.cpp feherjatekos.cpp feketejatekos.cpp jatektabla.cpp
