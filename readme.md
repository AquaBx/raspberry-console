

https://github.com/AquaBx/raspberry-console/assets/35542432/7b45ecbe-fb21-44f1-9c8d-66e7386b8d4b


# Compiler le code

Installer la toolchain du Raspberry Pico, pour [Windows](https://github.com/raspberrypi/pico-setup-windows/releases/)

## Copie du dépot
```
git clone https://github.com/AquaBx/raspberry-console
cd raspberry-console
```

## Copie des fichiers de configuration
⚠️ si les dossiers ne sont pas configurer dans le path, il se peut que les commandes fassent une erreur
vous pouvez sinon copier les dossiers manuellement.
```
copy ${env:PICO_SDK_PATH}\external\pico_sdk_import.cmake .
copy ${env:PICO_EXAMPLES_PATH}\pico-examples\.vscode . -recurse
```

Vous n'avez plus qu'à build à partir de l'extension CMAKE de vscode.

# Composant

|||
|---|---|
| [Raspberry Pi Pico H](https://www.gotronic.fr/art-carte-raspberry-pi-pico-h-35658.htm) | ![image](https://github.com/AquaBx/raspberry-console/assets/35542432/b921d503-fbb7-4496-a244-504bc605883c) |
| [Arduino Shield ILI9341 TF028](https://www.gotronic.fr/art-shield-ecran-tactile-2-8-tf028-28507.htm) | ![image](https://github.com/AquaBx/raspberry-console/assets/35542432/de91fb03-e3b7-4ff7-a440-d8dfd62801d7) |
| [Module HP SKU00060](https://www.gotronic.fr/art-module-hp-sku00060-28208.htm) | ![image](https://github.com/AquaBx/raspberry-console/assets/35542432/8f036b66-a1d8-41c5-88a5-9a532d69bc62)

![tpelek2_bb](https://github.com/AquaBx/raspberry-console/assets/35542432/559c41d1-3205-416c-bba8-596fc56d3637)
