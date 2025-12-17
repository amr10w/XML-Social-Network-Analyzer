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

  // ilovexml verify -i <input_file> [-f -o <output_file>]
  // ilovexml format -i <input_file> -o <output_file>
  // ilovexml mini -i <input_file> -o <output_file>
  // ilovexml json -i <input_file> -o <output_file>
  // ilovexml compress -i <input_file> -o <output_file>
  // ilovexml decompress -i <input_file> -o <output_file>
  // ilovexml draw -i <input_file> -o <output_file>
  // ilovexml most_active -i <input_file>
  // ilovexml most_influencer -i <input_file>
  // ilovexml mutual -i <input_file> -ids <id1>,<id2>,...
  // ilovexml suggest -i <input_file> -id <id>
  // ilovexml search -w <word> -i <input_file>
  // ilovexml search -t <topic> -i <input_file>


  return cli.handle(argc, argv);
}