function plotfiles( pcfile, ptext)

    if nargin < 2 || isempty(ptext)
        ptext = true;
    end
    pmarkersize=5;

    % get data
    data  = hdf5read( pcfile, '/project');
    if (size(data,2) ~= 2) && (size(data,2) ~= 3)
        error('plot only with 2D or 3D');
    end
    label = hdf5read( pcfile, '/files');
    datatxt=cell(1,size(data,1));
    for i=1:numel(datatxt)
        datatxt{i} = char(label(i).data);
    end

    % create plot
    figure;
    grid on;
    hold on;
    set(datacursormode(gcf), 'DisplayStyle','datatip', 'SnapToDataVertex','off','Enable','on', 'UpdateFcn',{@showlabel,datatxt});

    if size(data,2) == 2
        plot(data(:,1), data(:,2), '.', 'MarkerSize',pmarkersize);
    elseif size(data,2) == 3
        plot3(data(:,1), data(:,2), data(:,3), '.', 'MarkerSize',pmarkersize);
    end

    set(gca,'fontsize',1);
    
    if size(data,2) == 3
        view([-45 45]);
    end

function txt = showlabel(varargin)
    label = varargin{3};
    txt   = label{get(varargin{2}, 'DataIndex')};