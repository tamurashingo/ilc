<?xml version="1.0" encoding="UTF-8" ?>
<!-- @(#) $Id: ilut.xsl,v 1.1 2008/05/31 06:09:13 shingo Exp $ -->
<!-- Copyright (c) 2008, 2017 tamura shingo-->
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"
            version="1.0"
            encoding="UTF-8"
            omit-xml-declaration="yes"
            standalone="yes"
            doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN"
            doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"
            indent="yes"
            media-type="text/html" />

<xsl:template match="/">
  <html>
    <head>
      <title>unit test report</title>
      <style types="text/css">
        .graph {
          position: relative;
          width: 200px;
          border: 1px solid #000000;
          background: #d60000;
          padding: 0px;
        }
        .graph .bar {
          display: block;
          position: relative;
          background: #00D600;
          text-align: center;
          color: #FFFFFF;
          height: 2em;
          line-height: 2em;
        }
        .graph .bar span { position: absolute; left: 1em; }

        body {
          font:normal verdana,arial,helvetica;
          color:#000000;
        }
        body {
          font:normal 68% verdana,arial,helvetica;
          color:#000000;
        }
        table {
          width:100%;
          border:0;
        }
        table tr td {
          font-size: 100%;
          background:#eeeee0;
        }
        table tr th {
          font-size: 100%;
          text-align:center;
          background:#a6caf0;
        }
        h1 {
          margin: 0px 0px 5px; font: 165% verdana,arial,helvetica;
        }
        h2 {
          margin-top: 1em; margin-bottom: 0.5em; font: bold 125% verdana,arial,helvetica;
        }
        h3 {
          margin-bottom: 0.5em; font: bold 115% verdana,arial,helvetica;
        }
        h4 {
          margin-top: 0px;
          margin-bottom: 0.5em; font: bold 100% verdana,arial,helvetica;
          color: blue;
        }
        h5 {
          margin-bottom: 0.5em; font: bold 100% verdana,arial,helvetica;
          font-size: 95%;
        }
        h6 {
          margin-bottom: 0px; font: bold 100% verdana,arial,helvetica;
        }
        .success {
          background:green;
          color:white;
          text-align:center;
        }
        .failure {
          background:red;
          font-weight:bold;
          color:white;
          text-align:center;
        }
		.ngcomment {
		  color:red;
		}
      </style>
    </head>
    <body>
      <h1>
        unit test report
      </h1>

      <h2>
        Summary
      </h2>
      <xsl:variable name="Total_Summary" select="count(/unittest/testcase/test)" />
      <xsl:variable name="Success_Summary" select="count(/unittest/testcase/test[result=&quot;success&quot;])" />
      <xsl:variable name="Failure_Summary" select="count(/unittest/testcase/test[result!=&quot;success&quot;])" />
      <xsl:variable name="Rate_Summary" select="floor($Success_Summary div $Total_Summary * 100)" />
      <table>
        <tr><th>Total</th><th>Success</th><th>Failure</th><th>% Success</th></tr>
        <tr>
          <td><xsl:value-of select="$Total_Summary" /></td>
          <td><xsl:value-of select="$Success_Summary" /></td>
          <td><xsl:value-of select="$Failure_Summary" /></td>
          <td>
            <div class="graph">
              <xsl:element name="div">
                <xsl:attribute name="class"><xsl:text>bar</xsl:text></xsl:attribute>
                <xsl:attribute name="style">
                  <xsl:text>width:</xsl:text><xsl:value-of select="$Rate_Summary" /><xsl:text>%;</xsl:text>
                </xsl:attribute>
				<xsl:value-of select="$Rate_Summary" /><xsl:text>%</xsl:text>
              </xsl:element>
              <!-- <div class="bar" style="width:xx%;"></div> -->
            </div>
          </td>
        </tr>
      </table>

      <xsl:apply-templates />

      <br />
      <hr />
	  Copyright &#169; 2008, 2017 tamura shingo
	  <br />

    </body>
  </html>
</xsl:template>

<xsl:template match="unittst">
  <xsl:apply-templates />
</xsl:template>

<xsl:template match="testcase">

  <br />
  <hr />

  <h3>
	test case : <xsl:value-of select="name" />
  </h3>

  <xsl:variable name="Total_Summary"   select="count(test)" />
  <xsl:variable name="Success_Summary" select="count(test[result=&quot;success&quot;])" />
  <xsl:variable name="Failure_Summary" select="count(test[result!=&quot;success&quot;])" />
  <xsl:variable name="Rate_Summary"    select="floor($Success_Summary div $Total_Summary * 100)" />
  <table>
    <tr><th>Total</th><th>Success</th><th>Failure</th><th>% Success</th></tr>
    <tr>
      <td><xsl:value-of select="$Total_Summary" /></td>
      <td><xsl:value-of select="$Success_Summary" /></td>
      <td><xsl:value-of select="$Failure_Summary" /></td>
      <td>
        <div class="graph">
          <xsl:element name="div">
            <xsl:attribute name="class"><xsl:text>bar</xsl:text></xsl:attribute>
            <xsl:attribute name="style">
              <xsl:text>width:</xsl:text><xsl:value-of select="$Rate_Summary" /><xsl:text>%;</xsl:text>
            </xsl:attribute>
			<xsl:value-of select="$Rate_Summary" /><xsl:text>%</xsl:text>
          </xsl:element>
          <!-- <div class="bar" style="width:xx%;"></div> -->
        </div>
      </td>
    </tr>
  </table>

  <br />

  <table>
    <tr><th>test</th><th>start</th><th>end</th><th>result</th><th>comment</th></tr>
    <xsl:apply-templates />
  </table>

</xsl:template>

<xsl:template match="test">
  <tr>
    <td><xsl:value-of select="name" /></td>
    <td><xsl:value-of select="start" /></td>
    <td><xsl:value-of select="end" /></td>
    <xsl:choose>
      <xsl:when test="result=&quot;success&quot;">
        <td class="success"><xsl:value-of select="result" /></td>
		<td><xsl:value-of select="comment" /></td>
      </xsl:when>
      <xsl:otherwise>
        <td class="failure"><xsl:value-of select="result" /></td>
		<td class="ngcomment"><xsl:value-of select="comment" /></td>
      </xsl:otherwise>
    </xsl:choose>
  </tr>
</xsl:template>


<xsl:template match="name">
</xsl:template>


</xsl:stylesheet>

