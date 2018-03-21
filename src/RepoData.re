type repo = {
    full_name: string,
    stargazers_count: int,
    html_url: string
};

open Json.Decode;

let parseRepoJson = (json: Js.Json.t): repo => {
    full_name: Json.Decode.field("full_name", Json.Decode.string, json),
    stargazers_count: Json.Decode.field("stargazers_count", Json.Decode.int, json),
    html_url: Json.Decode.field("html_url", Json.Decode.string, json)
  };

let parseReposResponseJson = json =>
    Json.Decode.field("items", Json.Decode.array(parseRepoJson), json);

let reposUrl = "https://api.github.com/search/repositories?q=topic%3Areasonml&type=Repositories";

let fetchRepos = () =>
    Js.Promise.(
        Bs_fetch.fetch(reposUrl)
            |> then_(Bs_fetch.Response.text)
            |> then_(
            jsonText =>
                resolve(parseReposResponseJson(Js.Json.parseExn(jsonText)))
            )
    );