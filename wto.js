var addon = require('bindings')('wto');

var rc = addon.wto("hello from node.js", 123);

console.log(`wto rc is ${rc}`);
