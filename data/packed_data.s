        .global DATA_CONSOLAS_FONT
        .global DATA_CONSOLAS_FONT_SIZE

        .global DATA_UNOTU16_PNG
        .global DATA_UNOTU16_PNG_SIZE

        .global DATA_UNOTU32_PNG
        .global DATA_UNOTU32_PNG_SIZE

        .global DATA_UNOTU64_PNG
        .global DATA_UNOTU64_PNG_SIZE

        .global DATA_COLORED_VERTICES_FRAG
        .global DATA_COLORED_VERTICES_FRAG_SIZE

        .global DATA_COLORED_VERTICES_VS
        .global DATA_COLORED_VERTICES_VS_SIZE

        .global DATA_TEXTURED_VERTICES_FRAG
        .global DATA_TEXTURED_VERTICES_FRAG_SIZE

        .global DATA_TEXTURED_VERTICES_VS
        .global DATA_TEXTURED_VERTICES_VS_SIZE

        .global DATA_TEXT_FRAG
        .global DATA_TEXT_FRAG_SIZE

        .global DATA_TEXT_VS
        .global DATA_TEXT_VS_SIZE

        .section .rodata
// CONSOLAS FONT
        DATA_CONSOLAS_FONT:
                .incbin "consolas.png"
        DATA_CONSOLAS_FONT_SIZE:
                .int DATA_CONSOLAS_FONT_SIZE - DATA_CONSOLAS_FONT
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
// COLORED VERTICES FRAG
        DATA_COLORED_VERTICES_FRAG:
                .incbin "colored_vertices.frag"
        DATA_COLORED_VERTICES_FRAG_SIZE:
                .int DATA_COLORED_VERTICES_FRAG_SIZE - DATA_COLORED_VERTICES_FRAG
// COLORED VERTICES VS
        DATA_COLORED_VERTICES_VS:
                .incbin "colored_vertices.vs"
        DATA_COLORED_VERTICES_VS_SIZE:
                .int DATA_COLORED_VERTICES_VS_SIZE - DATA_COLORED_VERTICES_VS
// TEXTURED VERTICES FRAG
        DATA_TEXTURED_VERTICES_FRAG:
                .incbin "textured_vertices.frag"
        DATA_TEXTURED_VERTICES_FRAG_SIZE:
                .int DATA_TEXTURED_VERTICES_FRAG_SIZE - DATA_TEXTURED_VERTICES_FRAG
// TEXTURED VERTICES VS
        DATA_TEXTURED_VERTICES_VS:
                .incbin "textured_vertices.vs"
        DATA_TEXTURED_VERTICES_VS_SIZE:
                .int DATA_TEXTURED_VERTICES_VS_SIZE - DATA_TEXTURED_VERTICES_VS
// TEXT FRAG
        DATA_TEXT_FRAG:
                .incbin "text.frag"
        DATA_TEXT_FRAG_SIZE:
                .int DATA_TEXT_FRAG_SIZE - DATA_TEXT_FRAG
// TEXT VS
        DATA_TEXT_VS:
                .incbin "text.vs"
        DATA_TEXT_VS_SIZE:
                .int DATA_TEXT_VS_SIZE - DATA_TEXT_VS
