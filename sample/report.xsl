<?xml version="1.0" encoding="UTF-8" ?>
<!-- @(#) $Id: report.xsl,v 1.3 2008/05/31 06:07:50 shingo Exp $ -->
<!-- Copyright (c) 2007-2008, 2017 tamura shingo -->
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
      <title>Coverage Report</title>
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

        .Passed {
          background:green;
          color:white;
          text-align:center;
        }
        .NotYet {
          background:red;
          font-weight:bold;
          color:white;
          text-align:center;
        }
      </style>
    </head>
    <body>
      <h1>
        Coverage Report
      </h1>

      <hr />

      <h2>
        Summary
      </h2>
      <xsl:variable name="Total_Summary" select="count(coverage_report/coverage)" />
      <xsl:variable name="Covered_Summary" select="count(coverage_report/coverage[@result=&quot;true&quot;])" />
      <xsl:variable name="Rate_Summary" select="floor($Covered_Summary div $Total_Summary * 100)" />
      <table>
        <tr><th></th><th>Covered</th><th>Total</th><th>% Covered</th></tr>
        <tr>
          <td>All Files</td>
          <td><xsl:value-of select="$Covered_Summary" /></td>
          <td><xsl:value-of select="$Total_Summary"   /></td>
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
      <hr />
      <br />

      <h2>
        Files
      </h2>
      <table>
        <tr>
          <th>File</th>
          <th>Covered</th>
          <th>Total</th>
          <th>% Covered</th>
        </tr>

        <xsl:for-each select="coverage_report/coverage">
          <xsl:variable name="File_File"    select="@source" />
          <!-- 同じsourceが前方にあれば、出力済みとする（グルーピング処理） -->
          <xsl:if test="not(preceding-sibling::coverage[@source=$File_File])">
            <xsl:variable name="Total_File" select="count(/coverage_report/coverage[@source=$File_File])" />
            <xsl:variable name="Covered_File" select="count(/coverage_report/coverage[@source=$File_File and @result=&quot;true&quot;])" />
            <xsl:variable name="Rate_File" select="floor($Covered_File div $Total_File * 100)" />
            <tr>
              <td>
                <xsl:element name="a">
                  <xsl:attribute name="href">
                    <xsl:text>#</xsl:text><xsl:value-of select="$File_File" />
                  </xsl:attribute>
                  <xsl:value-of select="$File_File" />
                </xsl:element>
                <!-- <a href="#file">file</a> -->
              </td>
              <td>
                <xsl:value-of select="$Covered_File" />
              </td>
              <td>
                <xsl:value-of select="$Total_File" />
              </td>
              <td>
                <div class="graph">
                  <xsl:element name="div">
                    <xsl:attribute name="class"><xsl:text>bar</xsl:text></xsl:attribute>
                    <xsl:attribute name="style">
                      <xsl:text>width:</xsl:text><xsl:value-of select="$Rate_File" /><xsl:text>%;</xsl:text>
				  </xsl:attribute>
				  <xsl:value-of select="$Rate_File" /><xsl:text>%</xsl:text>
                  </xsl:element>
                  <!-- <div class="bar" style="width:xx%;"></div> -->
                </div>
              </td>
            </tr>
          </xsl:if>
        </xsl:for-each>
      </table>

      <br />
      <hr />
      <br />

      <h2>
        Functions By File
      </h2>

      <xsl:for-each select="coverage_report/coverage">
        <xsl:variable name="File_Func" select="@source" />
        <!-- ファイル名でグルーピング -->
        <xsl:if test="not(preceding-sibling::coverage[@source=$File_Func])">
          <h3>
            <xsl:element name="a">
              <xsl:attribute name="name"><xsl:value-of select="$File_Func" /></xsl:attribute>
              <xsl:value-of select="$File_Func" />
            </xsl:element>
            <!-- <a name="src.c">src.c</a> -->
          </h3>

          <table>
            <tr><th>Function</th><th>Covered</th><th>Total</th><th>% Covered</th></tr>

            <xsl:for-each select="/coverage_report/coverage[@source=$File_Func]">
              <xsl:variable name="Func_Func" select="@function" />
              <!-- 関数名でグルーピング -->
              <xsl:if test="not(preceding-sibling::coverage[@source=$File_Func and @function=$Func_Func])">
                <xsl:variable name="Total_Func" select="count(/coverage_report/coverage[@source=$File_Func and @function=$Func_Func])" />
                <xsl:variable name="Covered_Func" select="count(/coverage_report/coverage[@source=$File_Func and @function=$Func_Func and @result=&quot;true&quot;])" />
                <xsl:variable name="Rate_Func" select="floor($Covered_Func div $Total_Func * 100)" />
                <tr>
                  <td>
                    <xsl:element name="a">
                      <xsl:attribute name="href">
                        <xsl:text>#</xsl:text><xsl:value-of select="$Func_Func" />
                      </xsl:attribute>
                      <xsl:value-of select="$Func_Func" />
                    </xsl:element>
                    <!-- <a href="#func">func</a> -->
                  </td>
                  <td><xsl:value-of select="$Covered_Func" /></td>
                  <td><xsl:value-of select="$Total_Func" /></td>
                  <td>
                    <div class="graph">
                      <xsl:element name="div">
                        <xsl:attribute name="class"><xsl:text>bar</xsl:text></xsl:attribute>
                        <xsl:attribute name="style">
                          <xsl:text>width:</xsl:text><xsl:value-of select="$Rate_Func" /><xsl:text>%;</xsl:text>
                        </xsl:attribute>
						<xsl:value-of select="$Rate_Func" /><xsl:text>%</xsl:text>
                      </xsl:element>
                      <!-- <div class="bar" style="width:xx%;"></div> -->
                    </div>
                  </td>
                </tr>
              </xsl:if>
            </xsl:for-each>

          </table>

		  <br />

        </xsl:if>
      </xsl:for-each>

      <br />
      <hr />
      <br />

      <h2>
        Function Details
      </h2>

      <xsl:for-each select="coverage_report/coverage">
        <xsl:variable name="File_Detail" select="@source" />
        <!-- ファイル名でグルーピング -->
        <xsl:if test="not(preceding-sibling::coverage[@source=$File_Detail])">
          <h3>
            <xsl:value-of select="$File_Detail" />
          </h3>
          <table>
            <tr><th>Function</th><th>Line</th><th>Result</th></tr>

            <xsl:for-each select="/coverage_report/coverage[@source=$File_Detail]">
              <xsl:variable name="Func_Detail" select="@function" />
              <!-- 関数名でグルーピング -->
              <xsl:if test="not(preceding-sibling::coverage[@source=$File_Detail and @function=$Func_Detail])">
                <xsl:for-each select="/coverage_report/coverage[@source=$File_Detail and @function=$Func_Detail]">
                  <xsl:sort select="@line" data-type="number" />

                  <tr>
                    <td>
                      <xsl:element name="a">
                        <xsl:attribute name="name"><xsl:value-of select="$Func_Detail" /></xsl:attribute>
                        <xsl:value-of select="$Func_Detail" />
                      </xsl:element>
                      <!-- <a name="func">func</a> -->
                    </td>
                    <td><xsl:value-of select="@line" /></td>

                    <!-- result列の描画 -->
                    <xsl:choose>
                      <xsl:when test="@result=&quot;true&quot;">
                        <td class="Passed">passed</td>
                      </xsl:when>
                      <xsl:otherwise>
                        <td class="NotYet">not</td>
                      </xsl:otherwise>
                    </xsl:choose>
                  </tr>
                </xsl:for-each>


              </xsl:if>
            </xsl:for-each>

          </table>

		  <br />

        </xsl:if>
      </xsl:for-each>

	  <br />
	  <br />
	  <hr />
	  Copyright &#169; 2007-2008, 2017 tamura shingo
	  <br />

    </body>
  </html>
</xsl:template>

</xsl:stylesheet>


