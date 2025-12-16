#include "CLICommands.h"

int main(int argc, char *argv[])
{
  // CASE 1 — user passed CLI arguments
  CLICommands cli;

  // Documentation from 'main' branch
  // ilovexml verify -i test.xml
  // ilovexml mini -i in.xml -o out.xml
  // ilovexml json -i test.xml -o test.json
  // ilovexml compress -i sample.xml -o sample.comp
  // ilovexml decompress -i sample.comp -o sample_decompressed.xml

  return cli.handle(argc, argv);
}