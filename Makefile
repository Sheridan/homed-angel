BUILD_DIR = .build

mkpath:
	-@mkdir -p ${BUILD_DIR}/${subdir}

release: subdir=release
release: mkpath
	cd ${BUILD_DIR}/${subdir} && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd ${BUILD_DIR}/${subdir} && make -j`lscpu | grep "CPU(s)" | head -n1 | awk '{ print $$2 }'`
	cd ${BUILD_DIR}/${subdir} && cpack -G DEB

debug: subdir=debug
debug: mkpath
	reset
	cd ${BUILD_DIR}/${subdir} && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd ${BUILD_DIR}/${subdir} && make -j`lscpu | grep "CPU(s)" | head -n1 | awk '{ print $$2 }'`

clean:
	-rm -rf ${BUILD_DIR}/debug
	-rm -rf ${BUILD_DIR}/release
	-rm -rf log

run_debug: debug
	#reset && rm -f debug.log && ${BUILD_DIR}/debug/homed-angel -c ./homed-angel.json -s ${PWD}/scripts --verbose | tee debug.log
	reset && rm -f debug.log && ${BUILD_DIR}/debug/homed-angel -c ./homed-angel.json -s ${PWD}/scripts --verbose

run_release: release
	#reset && rm -f release.log && ${BUILD_DIR}/release/homed-angel -c ./homed-angel.json -s ${PWD}/scripts --verbose | tee debug.log
	reset && rm -f release.log && ${BUILD_DIR}/release/homed-angel -c ./homed-angel.json -s ${PWD}/scripts --verbose
