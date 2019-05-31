all:
	gcc xml_parser.c -o xml_parser -Wall -Wextra -pedantic -g `xml2-config --cflags --libs`
