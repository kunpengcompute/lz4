# Release Notes

## Version Mapping

### Product Version Information

<a name="table62675726"></a>
<table><tbody><tr id="row41561572"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.1.1"><p id="p11044137"><a name="p11044137"></a><a name="p11044137"></a>Product Name</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.1.1 "><p id="p1597721693713"><a name="p1597721693713"></a><a name="p1597721693713"></a>Kunpeng BoostKit</p>
</td>
</tr>
<tr id="row24726251"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.2.1"><p id="p56669300"><a name="p56669300"></a><a name="p56669300"></a>Product Version</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.2.1 "><p id="p11923034"><a name="p11923034"></a><a name="p11923034"></a><span id="text189831542174711"><a name="text189831542174711"></a><a name="text189831542174711"></a>26.1.RC1</span></p>
</td>
</tr>
<tr id="row1930811471892"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.3.1"><p id="p2030912172097"><a name="p2030912172097"></a><a name="p2030912172097"></a>Software Name</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.3.1 "><p id="p173091217"><a name="p173091217"></a><a name="p173091217"></a>LZ4 (Kunpeng optimized)</p>
</td>
</tr>
<tr id="row193081171892"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.3.1"><p id="p2030912172097"><a name="p2030912172097"></a><a name="p2030912172097"></a>Software Version</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.3.1 "><p id="p1730913179911"><a name="p1730913179911"></a><a name="p1730913179911"></a>V1.0.0</p>
</td>
</tr>
<tr id="row1930811171892"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.3.1"><p id="p2030912172097"><a name="p2030912172097"></a><a name="p2030912172097"></a>OSS Version</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.3.1 "><p id="p1730912179911"><a name="p1730912179911"></a><a name="p1730912179911"></a>LZ4 v1.9.4</p>
</td>
</tr>
</tbody>
</table>

### OS, Compiler, and CPU

|OS|CPU|Compiler|
|--|--|--|
|openEuler 22.03 LTS SP3|Kunpeng 920|Clang 16.0.6 or later<br>GCC 9.0 or later|

## Change Description

### V1.0.0

**New Features**

|Feature|Description|
|--|--|
|Kunpeng LZ4 compression performance optimization|Performance optimizations are applied to the core LZ4 compression and decompression paths on the AArch64 platform. These optimizations include NEON vectorized memory copy, branch prediction, data prefetch, loop alignment, adaptive hashing and stepping, and compilation option optimizations.|

## Documentation

### V1.0.0 Documentation

<a name="table41916133"></a>
<table><thead align="left"><tr id="row28804032"><th class="cellrowborder" valign="top" width="35.52%" id="mcps1.1.4.1.1"><p id="p4697041"><a name="p4697041"></a><a name="p4697041"></a>Document Name</p>
</th>
<th class="cellrowborder" valign="top" width="47.52%" id="mcps1.1.4.1.2"><p id="p44916036"><a name="p44916036"></a><a name="p44916036"></a>Description</p>
</th>
<th class="cellrowborder" valign="top" width="16.96%" id="mcps1.1.4.1.3"><p id="p14320308"><a name="p14320308"></a><a name="p14320308"></a>Delivery Method</p>
</th>
</tr>
</thead>
<tbody><tr id="row19094280"><td class="cellrowborder" valign="top" width="35.52%" headers="mcps1.1.4.1.1 "><p id="p1341193722116"><a name="p1341193722116"></a><a name="p1341193722116"></a>Release Notes</p>
</td>
<td class="cellrowborder" valign="top" width="47.52%" headers="mcps1.1.4.1.2 "><p id="p2042183752117"><a name="p2042183752117"></a><a name="p2042183752117"></a>Provides information about each release of Kunpeng LZ4 optimized based on open-source LZ4.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p83760545399"><a name="p83760545399"></a><a name="p83760545399"></a>Open-source repository</p>
</td>
</tr>
<tr id="row19739145124012"><td class="cellrowborder" valign="top" width="35.52%" headers="mcps1.1.4.1.1 "><p id="p1045512617409"><a name="p1045512617409"></a><a name="p1045512617409"></a>Quick Start</p>
</td>
<td class="cellrowborder" valign="top" width="47.52%" headers="mcps1.1.4.1.2 "><p id="p15918183742018"><a name="p15918183742018"></a><a name="p15918183742018"></a>Provides quick start examples and compilation instructions for Kunpeng LZ4.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p1074017515408"><a name="p1074017515408"></a><a name="p1074017515408"></a>Open-source repository</p>
</td>
</tr>
<tr id="row1941037152117"><td class="cellrowborder" valign="top" width="35.52%" headers="mcps1.1.4.1.1 "><p id="p5143115122016"><a name="p5143115122016"></a><a name="p5143115122016"></a>Installation Guide</p>
</td>
<td class="cellrowborder" valign="top" width="47.52%" headers="mcps1.1.4.1.2 "><p id="p1914345202019"><a name="p1914345202019"></a><a name="p1914345202019"></a>Provides detailed instructions on configuring, compiling, and installing Kunpeng LZ4.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p18376105483918"><a name="p18376105483918"></a><a name="p18376105483918"></a>Open-source repository</p>
</td>
</tr>
<tr id="row883510919404"><td class="cellrowborder" valign="top" width="35.52%" headers="mcps1.1.4.1.1 "><p id="p16281135814010"><a name="p16281135814010"></a><a name="p16281135814010"></a>API Reference</p>
</td>
<td class="cellrowborder" valign="top" width="47.52%" headers="mcps1.1.4.1.2 "><p id="p991893772013"><a name="p991893772013"></a><a name="p991893772013"></a>Provides descriptions and definitions of macros and functions related to NEON vectorized memory copy and hash optimization.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p7835129124011"><a name="p7835129124011"></a><a name="p7835129124011"></a>Open-source repository</p>
</td>
</tr>
</tbody>
</table>

### Obtaining Documentation

Visit the [open-source repository](https://gitcode.com/boostkit/lz4) to view or download related documents.
