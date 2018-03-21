[%%bs.raw {|
  require('./index.css');
|}];

type state = {repoData: option(array(RepoData.repo))};
type action = 
  | Loaded(array(RepoData.repo))  ;

let component = ReasonReact.reducerComponent("App");

let dummyRepo: array(RepoData.repo) = [|
  {
    stargazers_count: 27,
    full_name: "jsdf/reason-react-hacker-news",
    html_url: "https://github.com/jsdf/reason-react-hacker-news"
  },
  {
    stargazers_count: 93,
    full_name: "reasonml/reason-tools",
    html_url: "https://github.com/reasonml/reason-tools"
  }
|];
let make = (_children) => {
  ...component,
  initialState: () => {
    repoData: None
  },
  didMount: (self) => {
    let handleReposLoaded = self.reduce(repoData => Loaded(repoData));

    RepoData.fetchRepos()
      |> Js.Promise.then_(repoData => {
          handleReposLoaded(repoData);
          Js.Promise.resolve();
        })
      |> ignore;

    ReasonReact.NoUpdate;
  },
  reducer: (action, _state) => {
    switch action {
    | Loaded(loadedRepo) => 
        ReasonReact.Update({
          repoData: Some(loadedRepo)
        })
    };
  },
  render: (self) => {
    let repoItems =
      switch (self.state.repoData) {
      | Some(repos) => ReasonReact.arrayToElement(
          Array.map(
            (repo: RepoData.repo) => <RepoItem key=repo.full_name repo=repo />,
            repos
          )
        )
      | None => ReasonReact.stringToElement("Loading")
      };

    <div className="App">
      <h1>{ReasonReact.stringToElement("Reason Projects")}</h1>
      {repoItems}
    </div>
  }
};