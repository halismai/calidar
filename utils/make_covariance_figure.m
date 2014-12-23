function make_covariance_figure(C)
  %function make_covariance_figure(C)

  imagesc(C);  axis image; axis off;
  colormap(flipud(gray));
  tt = strtrim(cellstr(num2str(C(:), '%0.2f')));
  [x,y] = meshgrid(1:size(C,1));
  hdle = text(x(:), y(:), tt(:),'horizontalalignment','center','fontsize',25,...
    'interpreter', 'latex');

  m = mean(get(gca,'clim'));
  tt_c = repmat(C(:) > m, 1, 3);
  set(hdle, {'color'}, num2cell(tt_c,2));

end

