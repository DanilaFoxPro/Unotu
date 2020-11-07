
        .global DATA_UNOTU16_PNG
        .global DATA_UNOTU16_PNG_SIZE

        .global DATA_UNOTU32_PNG
        .global DATA_UNOTU32_PNG_SIZE

        .global DATA_UNOTU64_PNG
        .global DATA_UNOTU64_PNG_SIZE

        .section .rodata
// UNOTU 16 PNG
        DATA_UNOTU16_PNG:
                .incbin "Unotu_16.png"
        DATA_UNOTU16_PNG_SIZE:
                .int DATA_UNOTU16_PNG_SIZE - DATA_UNOTU16_PNG
// UNOTU 32 PNG
        DATA_UNOTU32_PNG:
                .incbin "Unotu_32.png"
        DATA_UNOTU32_PNG_SIZE:
                .int DATA_UNOTU32_PNG_SIZE - DATA_UNOTU32_PNG
// UNOTU 64 PNG
        DATA_UNOTU64_PNG:
                .incbin "Unotu_64.png"
        DATA_UNOTU64_PNG_SIZE:
                .int DATA_UNOTU64_PNG_SIZE - DATA_UNOTU64_PNG
