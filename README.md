# ILカバレッジ計測ツール

## これは

C言語用のカバレッジ計測ツールです。

## 使い方

### ソースコード

コメントの形式でカバレッジ計測ポイントを埋め込みます。
`ILC:` というコメントが計測ポイントになります。
コロン以降は通常のコメントを書きます。

```c
int foo(int x, int y) {
    /* ILC: foo開始 */

    if (x < 0) {
        /* ILC: xがマイナス */
        y = y - x;
    }
    else {
        /* ILC: xが0以上 */
        y = y + x;
    }

    /* ILC: foo終了 */
    return y;
}
```

### ソースコンバート

`ilc`コマンドでソースをコンバートします。

```sh
ilc src/foo.c
```

そうすると、 `foo_ilc.c` というファイルが生成されます。

```c
int foo(int x, int y) {
    /* ILC:*/ __ilc_check( "foo.c:foo:2" ); /* foo開始 */

    if (x < 0) {
        /* ILC:*/ __ilc_check( "foo.c:foo:5" ); /* xがマイナス */
        y = y - x;
    }
    else {
        /* ILC:*/ __ilc_check( "foo.c:foo:9" ); /* xが0以上 */
        y = y + x;
    }

    /* ILC:*/ __ilc_check( "foo.c:foo:13" ); /* foo終了 */
    return y;
}
```

### ビルド

`__ilc_check` を自前で用意する、もしくは `libilc.a` を含めます。
`libilc.a` はカレントディレクトリに計測ポイントを通過したかどうかの結果(`ilc.dat`)を吐き出します。


## 結果

`dat2xml.awk` というawkスクリプトを用意しているので、 `ilc.dat` をXMLに変換することができます。
さらにXSLファイルを用意しているのでHTMLに変換することができます。



License
-------
Copyright &copy; 2007-2008, 2017 tamura shingo
Licensed under the [MIT License][MIT].

[MIT]: http://www.opensource.org/licenses/mit-license.php

