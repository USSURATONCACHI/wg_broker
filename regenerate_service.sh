export FILENAME=echo_skeleton

gdbus-codegen --generate-c-code=$FILENAME --interface-prefix=ussur.wg. EchoService.xml
mkdir -p src/gen
mkdir -p include/wg_broker/gen
mv $FILENAME.c src/gen/$FILENAME.c
mv $FILENAME.h include/wg_broker/gen/$FILENAME.h