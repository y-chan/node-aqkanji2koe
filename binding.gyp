{
  "targets": [
    {
      "target_name": "aqkanji2koe",
      "sources": [
        "aqkanji2koe.cc",
        "aqkanji2koe.h",
        "aqkanji2koe/common.h"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "conditions": [
        [
          "OS=='win'",
          {
            "sources": [
              "aqkanji2koe/win.cpp",
              "aqkanji2koe/win.h"
            ],
            "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": "2"
              },
            }
          }
        ],
        [
          "OS=='linux'",
          {
            "sources": [
              "aqkanji2koe/linux.cpp",
              "aqkanji2koe/linux.h"
            ]
          }
        ],
        [
          "OS=='mac'",
          {
            "sources": [
              "aqkanji2koe/linux.cpp",
              "aqkanji2koe/linux.h"
            ],
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
            }
          }
        ]
      ]
    }
  ]
}
