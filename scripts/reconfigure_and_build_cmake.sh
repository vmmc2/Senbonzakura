# Na raiz do seu projeto (ex: /home/vmmc2/Senbonzakura/)
cd ..
rm -rf build/        # Limpa o diretório de build completamente
mkdir build          # Cria o diretório de build vazio novamente
cd build             # Entra no diretório de build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/vmmc2/vcpkg/scripts/buildsystems/vcpkg.cmake # Configura
cmake --build .      # Constrói