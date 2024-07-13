add_library(printer INTERFACE
    printer.h
)

add_library(cryptopals::printer ALIAS printer)
