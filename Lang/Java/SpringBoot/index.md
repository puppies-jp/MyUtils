# Spring Boot

SpringBootについてまとめる
SpringBootはJavaのWebアプリケーションで多く使われるフレームワークである。

関連ライブラリとして以下があげられる

- lombok : Javaの冗長な定型コードをアノテーションで自動生成し、開発効率を向上させるライブラリ
Entity,Formクラスで使われるらしい

- thymeleaf: サーバーサイド HTML テンプレートエンジン の一つで、html内にコードを埋め込むことができる


用語として以下のLayerで覚えておくといい(DB系のWebアプリなら)

[Qiita(#SpringとMySQLの接続設定、登録・更新機能の実装
)](https://qiita.com/Jackoguro/items/fd4f6a0744718fe3ba69)
1. インストラクチャ層（データベースの操作処理）
    1. Entity : データベース内のテーブル1行に対応するクラス



    2. Repository: データベースへのデータ操作を行うクラス  
    (`CRUD`などはここ)

2. ドメイン層（サービス処理）

    実際に使用する処理内容を記述、
    **ここからRepositoryを呼び出す**

    1. interfaceの作成
    2. interfaceに合わせたクラスの作成

3. アプリケーション層（ビュー、コントローラー）の設定

    1. Viewクラスの実装  
       htmlに合わせてViewクラスを生成する
    2. Controllerクラスの実装  
       以下のイメージ
       - 画面遷移(URLのマッピング)
       - データの受け渡しに関する内容を記述(Viewからドメイン層にデータを渡すみたいな。。。)


なんとなくこんな感じの建付け

- Model
- Repository
- Service
    - Interface
    - InterfaceMode
- Applicateion
    - form
    - Controller


## SQLサーバにアクセスする


`url`に指定するDBまで指定すること

```
spring.datasource.url=jdbc:mysql://localhost:53306/mydatabase
spring.datasource.username=admin
spring.datasource.password=password
spring.datasource.driver-class-name=com.mysql.cj.jdbc.Driver

```

## アノテーション

### インストラクチャ層（データベースの操作処理）

<details>
<summary>
Entity
</summary>

```java
package com.example.demo.entity;

import org.springframework.data.annotation.Id;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data               // Getter,Setterが不要になる
@NoArgsConstructor  // デフォルトコンストラクターの自動生成
@AllArgsConstructor // 全フィールドに対する初期化値を引数に取るコンストラクタを自動生成
public class Sample {
    @Id             // 主キーに当たるフィールドに付与する(今回はid)に付与
    private Integer id;
    private String name;
    private Integer gender;
    private Integer age;
}

```
</details>

<details>
<summary>
Repository
</summary>

```java
package com.example.demo.repository;

import org.springframework.data.jdbc.repository.query.Modifying;
import org.springframework.data.jdbc.repository.query.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import com.example.demo.entity.Sample;

public interface SampleCrudRepository extends CrudRepository<Sample, Integer> {
	@Modifying
	@Query(value = "INSERT INTO sample VALUES(:id, :name, :gender, :age)")
	void InsertData(@Param("id") Integer id, 
			       @Param("name") String name, 
			       @Param("gender") Integer gender,
			       @Param("age") Integer age);
}
```

</details>

### ドメイン層（サービス処理）

<details>
<summary>
interface(impl)
</summary>

```java
package com.example.demo.service;

import java.util.Optional;

import com.example.demo.entity.Sample;

public interface SampleService {
	
	/* 全件取得 */
	Iterable<Sample> SelectAll();
	
	/**
	 * id(主キー)をキーにして1件取得する
	 * 戻り値型はOptional型を使用 
	 * isPresent()を使用でき、値がある場合はtrueを返す
	 */
	Optional<Sample> SlectOneById(Integer id);
	
	/* 取得したデータをDBにInsertする */
    void InsertSample(Sample sample);
    
    /* データを更新する */
    void UpdateSample(Sample sample);
}
```

</details>

<details>
<summary>
interface実装
</summary>

```java
@Service
@Transactional
public class SampleServiceImpl implements SampleService {
	
	@Autowired
	SampleCrudRepository repository;
	
	@Override
	public Iterable<Sample> SelectAll() {
		return repository.findAll();
	}

	@Override
	public Optional<Sample> SlectOneById(Integer id) {
		return repository.findById(id);
	}

	@Override
	public void InsertSample(Sample sample) {
		repository.InsertData(sample.getId(), 
							  sample.getName(), 
							  sample.getGender(), 
							  sample.getAge());
	}

	@Override
	public void UpdateSample(Sample sample) {
		repository.save(sample);
	}
}
```

</details>

### コントローラ層

<details>
<summary>
formクラス/Form.html
</summary>

```java
package com.example.demo.form;

import javax.validation.constraints.NotBlank;
import javax.validation.constraints.NotNull;

import org.hibernate.validator.constraints.Length;
import org.hibernate.validator.constraints.Range;

import lombok.Data;
@Data
public class SampleForm {
	@NotNull
    @Min(0)                 // 0以上の値を入力可能
	private Integer id;
	
	@NotBlank
	@Length(min=1, max=100) // 1～100文字の範囲で入力可能
	private String name;
	
	private Integer gender;
	
	@NotNull
	@Range(min=0, max=120)  // 1～120歳の範囲で入力可能
	private Integer age;
	
	/* 「登録」か「更新」判定用 */
	private Boolean NewSample;
}

```

```html
<!DOCTYPE html>
<html xmlns:th="http://thymeleaf.org">
<head>
<meta charset="UTF-8">
<title>サンプル</title>
</head>
<body>
	<table border="1" th:unless="${#lists.isEmpty(list)}" style="table-layout: fixed;">
		<tr>
			<th>ID</th><th>名前</th><th>性別</th>
            <th>年齢</th><th>編集</th>
		</tr>
		<tr th:each="obj : ${list}" align="center">
			<td th:text="${obj.id}"></td>
			<td th:text="${obj.name}"></td>
			<td th:text="${obj.gender} == 0? '男性' : '女性'"></td>
			<td th:text="${obj.age}"></td>
			<!-- 編集ボタン -->
			<td>
				<form method="get" th:action="@{sample/{id}(id=${obj.id})}">
					<input type="submit" value="編集">
				</form>
			</td>
		</tr>
	</table>
	<!-- 新規登録 -->
	<!-- newSampleがtrueの場合、新規登録処理、そうではない場合、更新処理にそれぞれ変更します -->
	<form method="post"
	       th:action="${sampleForm.NewSample}? @{/sample/insert} : @{/sample/update}"
	       th:object="${sampleForm}">
	       <hr>
	       <label>ID　：</label>
	       <input type="number" th:field="*{id}">
	       <br>
	       <label>名前：</label>
	       <input type="text" th:field="*{name}">
	       <br>
	       <label>男性：</label>
	       <input type="radio" value=0 th:field="*{gender}">
	       <label>女性：</label>
	       <input type="radio" value=1 th:field="*{gender}">
	       <br>
	       <label>年齢：</label>
	       <input type="number" th:field="*{age}">
	       <br>
	       <input type="submit" value="送信">
	</form>
    <!-- 追加部分 -->
    <p th:unless="${sampleForm.newSample}">
		<a href="#" th:href="@{/sample}">一覧画面へ戻る</a>
	</p>
</body>
```

</details>

<details><summary>
コントローラクラス
</summary>

```java
package com.example.demo.controller;

import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import com.example.demo.entity.Sample;
import com.example.demo.form.SampleForm;
import com.example.demo.service.SampleService;

@RequestMapping("/sample")
@Controller
public class SampleController {

	@Autowired
	SampleService service;
	
	/* 「form-backing bean」の初期化 */
	@ModelAttribute
	public SampleForm setUpForm() {
		SampleForm form = new SampleForm();
		form.setGender(0);
		return form;
	}
	/* 一覧表示 */
	@GetMapping
	public String ShowList(SampleForm sampleForm, Model model) {
		sampleForm.setNewSample(true); // 新規登録設定(初期値をtrueとしておく) 
		Iterable<Sample> sample = service.SelectAll(); // DB内のデータを全件取得
		model.addAttribute("list", sample); 
		return "show";
	}
	
    /* 登録機能作成 */
	@PostMapping("/insert")
	public String insert(@Validated SampleForm sampleForm, BindingResult bindingResult,
			Model model, RedirectAttributes redirectAttributes) {
		/* FormからEntityに詰め替え */
		Sample sample = new Sample();
		sample.setId(sampleForm.getId());
		sample.setName(sampleForm.getName());
		sample.setGender(sampleForm.getGender());
		sample.setAge(sampleForm.getAge());
		/* 入力チェック */
		if( !bindingResult.hasErrors()) {
			service.InsertSample(sample);
            /* ★リダイレクトはこんな感じ */
			return "redirect:/sample";
		} else {
			/* エラーがある場合は、一覧表示処理を呼ぶ */
			return ShowList(sampleForm, model);
		}
	}
	
    /* 更新用画面の表示 */
	@GetMapping("/{id}")
	public String ShowUpdate(SampleForm sampleForm, @PathVariable Integer id, Model model) {
		/* 表示する行を取得 */
		Optional<Sample> sampleOpt = service.SlectOneById(id);
		/* SampleFormに入れ直す */
		Optional<SampleForm> sampleFormOpt = sampleOpt.map(t -> makeSampleForm(t));
		/* SampleFormがnullでなければ中身を取り出す */
		if(sampleFormOpt.isPresent()) {
			sampleForm = sampleFormOpt.get();
		}
		makeUpdateModel(sampleForm, model);
		return "show";
	}
	
	/* 更新用のModelを作成する */
	private void makeUpdateModel(SampleForm sampleForm, Model model) {
		model.addAttribute("id", sampleForm.getId());
		sampleForm.setNewSample(false);
		model.addAttribute("sampleForm", sampleForm);
	}
	
    /* idをキーにしてデータを更新する */
	@PostMapping("/update")
	public String update(@Validated SampleForm sampleForm, BindingResult result,
						 Model model, RedirectAttributes redirectAttributes) {
		Sample sample = MakeSample(sampleForm);
		if(!result.hasErrors()) {
			service.UpdateSample(sample);
    		return "redirect:/sample/" + sample.getId();
		} else {
			makeUpdateModel(sampleForm, model);
			return "show";
        }
	}
	
    /**
     * Form -> Entity Method
    **/
	private Sample MakeSample(SampleForm sampleForm) {
		Sample sample = new Sample();
		sample.setId(sampleForm.getId());
		sample.setName(sampleForm.getName());
		sample.setGender(sampleForm.getGender());
		sample.setAge(sampleForm.getAge());
		return sample;
	}
	
	/**
     * Entity -> Formメソッド
	 * makeQuizFormはquiz(エンティティクラス(DB関連クラス))からQuizForm(Formクラス)へ
	 * 値を代入しformを返している。
	 */
	private SampleForm makeSampleForm(Sample sample) {
		SampleForm form = new SampleForm();
		form.setId(sample.getId());
		form.setName(sample.getName());
		form.setGender(sample.getGender());
		form.setAge(sample.getAge());
		form.setNewSample(false);
		return form;
	}
}

```

</details>