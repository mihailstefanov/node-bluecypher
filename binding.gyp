{
  "targets": [
    {
      "target_name": "bluecypher",
      "sources": [ "cypher.cc", "cmac_object.cc", "aes.c", "cmac.c",
                    "aes_object.c"],
      "include_dirs": [
                        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
