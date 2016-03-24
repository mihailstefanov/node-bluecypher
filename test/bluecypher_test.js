const cypher = require('../index.js');

describe('CMAC', function() {
    it('generate CMAC', function() {
        var o = new cypher.CMac();
        o.key = new Buffer('00112233445566778899AABBCCDDEEFF','hex');
        o.update(new Buffer('001032547698badcfeed551a000ba304006813','hex'));
        console.log(o.final());
    });
});

describe('AES', function() {
    it('encrypt a buffer', function() {
        var a = new cypher.Aes();
        a.key = new Buffer('00112233445566778899AABBCCDDEEFF','hex');
        console.log(a.encrypt(new Buffer('00112233445566778899AABBCCDDEEFF','hex')));
    });
});
