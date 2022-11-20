const jsdom = require('jsdom');
const { JSDOM } = jsdom;
const dom = new JSDOM(`<html><body><div id="aaa">AAA<div></body></html>`);
const { document } = dom.window;
const jquery = require('jquery');
const $ = jquery(dom.window);

console.log($("#aaa"));
console.log($("#aaa").text());
console.log($("#aaa")[0].id);

