target:
	g++ shader/mobil_mantap.cpp shader/shader.cpp -o mobil_mantap_shader -lGL -lGLEW -lglfw -std=c++11
	g++ immediate/mobil_mantap.cpp -o mobil_mantap_immediate -lGL -lGLEW -lglfw -std=c++11

run:
	echo 'Command : make [runshader|runimmediate]'

runshader:
	./mobil_mantap_shader

runimmediate:
	./mobil_mantap_immediate
