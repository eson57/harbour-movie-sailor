#ifndef API_H
#define API_H

#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QMap>
#include <QMetaMethod>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

#include "apiworker.h"
#include "src/system.h"
#include "src/models/account.h"
#include "src/models/accountmoviesform.h"
#include "src/models/movie.h"
#include "src/models/searchpeopleform.h"
#include "src/models/discovermovie.h"
#include "src/models/settings.h"
#include "src/models/form.h"
#include "src/models/searchform.h"
#include "src/models/tv.h"

class Api : public QObject
{
    Q_OBJECT
public:
    enum WorkerName {
        Account = 0,
        ConfigurationCountries,
        ConfigurationDetails,
        ConfigurationLanguages,
        DiscoverMovies,
        Genres,
        Keywords,
        LoadMovie,
        LoadTv,
        MoviePeople,
        WatchMovieProviders,
        SearchCompanies,
        SearchPeople,
        RequestRefreshToken,
        CreateSession,
        ToggleFavoriteMovie,
        ToggleWatchlistMovie,
        AddRatingMovie,
        RemoveRatingTv,
        ToggleFavoriteTv,
        ToggleWatchlistTv,
        AddRatingTv,
        RemoveRatingMovie,
        FavoriteMovies,
        FavoriteTv,
        SearchMedia,
        RatedMovies,
        RatedTv,
        WatchlistMovies,
        WatchlistTv,
    };
    Api(class Account *account, Settings &settings, QObject *parent);

    RequestInfo* getRequestInfo(WorkerName name);

    void discoverMovies(const DiscoverMovie &form);
    void loadConfigurationCounries();
    void loadConfigurationDetails();
    void loadConfigurationLanguages();
    void loadMovie(int id);
    void loadTv(int id);
    void loadMovieGenres();
    void loadTVGenres();
    void loadKeywords(const QString query, int page);
    void loadWatchMovieProviders(const QString &region);
    void loadSearchPersons(const SearchPeopleForm &form);
    void requestRefreshToken();
    void createSessionId(const QByteArray &data);
    void loadAccount();

    void toggleFavorite(WorkerName workerName, const QString &type, int id, bool newValue);
    void toggleFavorite(const class Movie &movie);
    void toggleFavorite(const Tv &movie);

    void toggleWatchlist(WorkerName workerName, const QString &type, int id, bool newValue);
    void toggleWatchlist(const class Movie &movie);
    void toggleWatchlist(const Tv &movie);

    void addRating(WorkerName workerName, const QString &type, int id, int newValue);
    void addRating(const class Movie &movie, int rating);
    void addRating(const Tv &movie, int rating);

    void removeRating(WorkerName workerName, const QString &type, int id);
    void removeRating(const class Movie &movie);
    void removeRating(const Tv &movie);

    void getResource(WorkerName workerName, const Form &form);
    void searchMedia(WorkerName workerName, const SearchForm &form);

private:
    class Account *account;
    Settings &settings;
    QNetworkAccessManager networkManager;
    QUrl buildUrl(const QString &url, const QUrlQuery &query);
    QUrl buildUrl(const QString &url);
    QNetworkRequest buildRequest(const QUrl &url);
    QNetworkRequest buildRequest(const QString &endpoint);
    QNetworkRequest buildRequest(const QString &endpoint, const QUrlQuery &query);
    QString baseUrl;
    QString token;

    QMap<WorkerName, ApiWorker*> workers;
    void setupWorker(WorkerName name, const char *method);
    ApiWorker* getWorker(WorkerName name) const;
    QString getLanguage() const;
    QString getIncludeAdult() const;

signals:
    void configurationCountriesDone(QByteArray &);
    void configurationDetailsDone(QByteArray &);
    void configurationLanguagesDone(QByteArray &);
    void loadAccountDone(QByteArray &);
    void movieDone(QByteArray &);
    void loadTvDone(QByteArray &);
    void discoverMoviesDone(QByteArray &);
    void genresDone(QByteArray &);
    void keywordsDone(QByteArray &);
    void watchMovieProvidersDone(QByteArray &);
    void searchCompaniesDone(QByteArray &);
    void searchPersonsDone(QByteArray &);
    void requestRefreshTokenDone(QByteArray &);
    void createSessionDone(QByteArray &);

    void favoriteDone(QByteArray &);
    void toggleWatchlistDone(QByteArray &);
    void addRatingDone(QByteArray &);
    void removeRatingDone(QByteArray &);

    void toggleFavoriteTvDone(QByteArray &);
    void toggleWatchlistTvDone(QByteArray &);
    void addRatingTvDone(QByteArray &);
    void removeRatingTvDone(QByteArray &);

    void favoriteMoviesDone(QByteArray &);
    void favoriteTvDone(QByteArray &);
    void searchMediaDone(QByteArray &);
    void ratedMoviesDone(QByteArray &);
    void ratedTvDone(QByteArray &);
    void watchlistMoviesDone(QByteArray &);
    void watchlistTvDone(QByteArray &);

};

#endif // API_H
